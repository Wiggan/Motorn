cbuffer ConstantBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4 colorConstant;
	float time;
	float3 blank;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	//float2 uv : TEXCOORD0;
	float4 normal : TEXCOORD1;
};


VOut VShader(float4 position : POSITION, float4 color : COLOR, float4 normal : NORMAL/*, float2 uv : TEXCOORD0*/)
{
	VOut output;

	position.w = 1.0f;
	normal.w = 0.0f;
	
	output.position = mul(position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.normal = mul(normal, worldMatrix);
	
	output.color = color;
	//output.color = color*0;
	output.color[0] = 0.0f;
	//output.uv = uv;

	return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, /*float2 uv : TEXCOORD0, */float4 normal : TEXCOORD1) : SV_TARGET
{

	float3 lightDir = normalize(float3(-1.1f, -1.0f, 0.1f));
	float power = 1.0f - dot(lightDir, normal);
	if ( power < 0.0f ) {
		power = 0.2f;
	}
	color.rgb *= power;

	return color;

}




