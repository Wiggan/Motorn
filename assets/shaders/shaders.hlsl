struct DirectionalLight 
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
};
struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
};
cbuffer PerObjectBuffer : register(b0)
{
	matrix worldMatrix;
	Material material;
};
cbuffer PerFrameBuffer : register(b1)
{
	matrix viewMatrix;
	matrix projectionMatrix;
	DirectionalLight directionalLight;
	float3 cameraPosition;
	float pad1;
	double time;
	float2 pad2;
};

Texture2D meshTexture;

SamplerState samplerState0 {
	Filter = MIN_MAG_MIP_LINEAR;
};

struct VOut
{
	float4 positionH : SV_POSITION;
	float3 positionW : POSITION;
	float2 uv : TEXCOORD0;
	float4 normalW : TEXCOORD1;
};


VOut VShader(float4 position : POSITION, float4 color : COLOR, float4 normal : NORMAL, float2 uv : TEXCOORD0)
{
	VOut output;

	position.w = 1.0f;
	normal.w = 0.0f;
	output.positionH = mul(position, worldMatrix);
	output.positionH = mul(output.positionH, viewMatrix);
	output.positionH = mul(output.positionH, projectionMatrix);
	output.normalW = normalize(mul(normal, worldMatrix));
	output.uv = uv;
	output.positionW = mul(position, worldMatrix).xyz;
	return output;
}

void calculateDirectionalLight(in float3 pToCamera, in DirectionalLight pLight, in Material pMaterial, in float3 pNormal, out float4 pAmbient, out float4 pDiffuse, out float4 pSpecular)
{
	pAmbient = pMaterial.ambient * pLight.ambient;
	pDiffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	pSpecular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	float3 lightVector = -pLight.direction;
	float diffuseFactor = dot(lightVector, pNormal);
	if(diffuseFactor > 0.0f) {
		pDiffuse = diffuseFactor * pMaterial.diffuse * pLight.diffuse;
		float3 v = reflect(-lightVector, pNormal);
		float specularFactor = pow(max(dot(v, pToCamera), 0.0f), pMaterial.specular.w);
		pSpecular = specularFactor * pMaterial.specular * pLight.specular;
	}
}

float4 PShader(VOut input) : SV_TARGET
{
	float4 texColor = meshTexture.Sample(samplerState0, input.uv);
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	float4 A, D, S;
	
	float3 toCamera = normalize(cameraPosition - input.positionW);
	float3 lightDir = normalize(directionalLight.direction);
	calculateDirectionalLight(toCamera, directionalLight, material, input.normalW.xyz, A, D, S);

	ambient += A;
	diffuse += D;
	specular += S;
	
	float4 color = texColor * (ambient + diffuse) + specular;
	color.a = diffuse.a * texColor.a;
	//color = color*float4(cameraPosition,0.0f);
	return color;

}




