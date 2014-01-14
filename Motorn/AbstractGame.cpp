#include "AbstractGame.h"
#include "Camera.h"
#include "ResourceLoader.h"
#include "PointLightComponent.h"
#include "Timer.h"
const int WIDTH = 800;
const int HEIGHT = 600;
const float RAY_CAST_INTERVAL = 100;
AbstractGame::AbstractGame()
{
    DirectX::XMStoreFloat4x4(&mWorldTransform, DirectX::XMMatrixIdentity());
}



AbstractGame::~AbstractGame()
{
    std::cout << "Shutting down application" << std::endl;
    cleanD3D();
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void AbstractGame::initD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 1;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &swapchain,
        &dev,
        NULL,
        &devcon);

    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    HRESULT hr;
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    // Initialize the description of the depth buffer.
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // Set up the description of the depth buffer.
    depthBufferDesc.Width = WIDTH;
    depthBufferDesc.Height = HEIGHT;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer using the filled out description.
    hr = dev->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
    if ( FAILED(hr) ) {
        std::cout << "Failed to create depth buffer! " << hr << std::endl;
        return;
    }

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    // Initialize the description of the stencil state.
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create the depth stencil state.
    hr = dev->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
    if ( FAILED(hr) ) {
        std::cout << "Failed to create depth stencil state! " << hr << std::endl;
        return;
    }

    devcon->OMSetDepthStencilState(depthStencilState, 1);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    // Initailze the depth stencil view.
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // Create the depth stencil view.
    hr = dev->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
    if ( FAILED(hr) ) {
        std::cout << "Failed to create depth stencil view! " << hr << std::endl;
        return;
    }

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, depthStencilView);

    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = WIDTH;
    viewport.Height = HEIGHT;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    devcon->RSSetViewports(1, &viewport);

    ID3D11RasterizerState * rasterState;

    D3D11_RASTERIZER_DESC rasterizerState;
    rasterizerState.FillMode = D3D11_FILL_SOLID;
    rasterizerState.CullMode = D3D11_CULL_FRONT;
    rasterizerState.FrontCounterClockwise = true;
    rasterizerState.DepthBias = 0;
    rasterizerState.DepthBiasClamp = 0;
    rasterizerState.SlopeScaledDepthBias = 0;
    rasterizerState.DepthClipEnable = true;
    rasterizerState.ScissorEnable = false;
    rasterizerState.MultisampleEnable = false;
    rasterizerState.AntialiasedLineEnable = false;
    hr = dev->CreateRasterizerState(&rasterizerState, &rasterState);
    if ( FAILED(hr) ) {
        std::cout << "Failed to create raster state! " << hr << std::endl;
        return;
    }
    devcon->RSSetState(rasterState);

    D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
    ZeroMemory(&renderTargetBlendDesc, sizeof(renderTargetBlendDesc));
    renderTargetBlendDesc.BlendEnable = false;
    renderTargetBlendDesc.SrcBlend = D3D11_BLEND_ZERO;
    renderTargetBlendDesc.DestBlend = D3D11_BLEND_SRC_COLOR;
    renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
    renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE;


    D3D11_BLEND_DESC blendDesc;
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0] = renderTargetBlendDesc;

    ID3D11BlendState* blendState;
    ZeroMemory(&blendState, sizeof(blendState));
    dev->CreateBlendState(&blendDesc, &blendState);

    float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    devcon->OMSetBlendState(blendState, blendFactors,0xffffffff);


    pipelineInit();
}
void AbstractGame::renderFrame(double delta)
{
    // GAME LOGIC
    static double total = delta;
    total += delta;
    static float timeSinceLastRayCast = 0;
    timeSinceLastRayCast += delta;
    if ( timeSinceLastRayCast > RAY_CAST_INTERVAL ) {
        timeSinceLastRayCast = 0;
        mWorld->rayCast(mRayCastableEntities);
    }
    Timer::getInstance().update(delta);
    update(delta);
    // GAME RENDERING
    static const FLOAT clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    devcon->ClearRenderTargetView(backbuffer, clearColor);
    devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);;
    PerFrameBuffer constants;
    constants.projectionMatrix = Camera::getInstance().getProjection();
    constants.viewMatrix = Camera::getInstance().getView();
    constants.directionalLight = *mDirectionalLight;
    constants.pointLightCount = mPointLights.size();
    constants.fogColor = DirectX::XMFLOAT4( 0.7f, 0.7f, 0.7f, 1.0f );
    constants.fogStart = 5.0f;
    constants.fogRange = 5.0f;
    for ( int i = 0; i < constants.pointLightCount && i < 3; i++ ) {
        constants.pointLights[i] = mPointLights[i]->getPointLight();
    }
    constants.cameraPosition = Camera::getInstance().getPosition();
    //std::cout << Camera::getInstance().getPosition().x << ", " << Camera::getInstance().getPosition().y << ", " << Camera::getInstance().getPosition().z << ", " << std::endl;
    constants.time = 1234;
    setFrameConstants(constants);
    mWorld->draw();
    swapchain->Present(0, 0);
}
void AbstractGame::setFrameConstants(const PerFrameBuffer &constants) {
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(constants);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    ID3D11Buffer * buffer;
    HRESULT hr1 = dev->CreateBuffer(&bd, NULL, &buffer);
    if ( FAILED(hr1) ) {
        std::cout << "Failed to create buffer! " << hr1 << std::endl;
        std::cin.get();
        exit(1);
    }
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    hr1 = devcon->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if ( FAILED(hr1) ) {
        std::cout << "Failed mapping buffer to mapped resource!" << std::endl;
        std::cin.get();
        exit(1);
    }
    memcpy(mappedResource.pData, &constants, sizeof(constants));
    devcon->Unmap(buffer, 0);
    devcon->PSSetConstantBuffers(1, 1, &buffer);
    devcon->VSSetConstantBuffers(1, 1, &buffer);
    buffer->Release();
}
int checkForChangedResources(void *ptr) {
    while ( true ) {
        Sleep(1000);
        ResourceLoader::checkForChangedResources();
    }
    return 0;
}

bool AbstractGame::pipelineInit()
{
    mStuff.dev = dev;
    mStuff.devcon = devcon;
    ResourceLoader::init(mStuff);
    if ( !ResourceLoader::getShader("shaders")->load() ) {
        std::cout << "initial shader loading failed. Exiting";
        std::cin.get();
        exit(1);
    }

    SDL_Thread *thread;

    // Simply create a thread
    thread = SDL_CreateThread(checkForChangedResources, "ResourceCheckThread", (void *)NULL);

    if ( NULL == thread ) {
        std::cout << "Failed creating SDL thread for resource checking: " << SDL_GetError() << std::endl;
    }
    return true;
}
DirectX::XMINT2 AbstractGame::getWindowSize()
{
    DirectX::XMINT2 size;
    SDL_GetWindowSize(win, &size.x, &size.y);
    std::cout << "SDL Window size is " << size.x << "x" << size.y << std::endl;
    return size;
}

void AbstractGame::setRendererSize(const DirectX::XMINT2 &size)
{
    SDL_RenderSetLogicalSize(ren, size.x, size.y);
}

bool AbstractGame::initSDL() {
    using namespace std;
    cout << "Starting application and setting up SDL" << endl;
    if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 ) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        throw "SDL_Init failed";
    }
    win = SDL_CreateWindow("Hello World!", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if ( win == nullptr ) {
        cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        throw "SDL_CreateWindow failed";
    }
    ren = SDL_CreateRenderer(win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if ( ren == nullptr ) {
        cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        throw "SDL_CreateRenderer failed";
    }
    bmp = SDL_LoadBMP("bark 1.bmp");
    if ( bmp == nullptr ) {
        cout << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
        throw "SDL_LoadBMP failed";
    }
    tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_FreeSurface(bmp);
    if ( tex == nullptr ) {
        cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
        throw "SDL_CreateTextureFromSurface failed";
    }
    SDL_SetRelativeMouseMode(SDL_TRUE);
    // Get hWND from SDL
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if ( !SDL_GetWindowWMInfo(win, &wmi) ) throw "Coudln't get hwnd from SDL";
    hwnd = wmi.info.win.window;
}

void AbstractGame::startGame() {
    using namespace std;
    try {
        initSDL();
        initD3D(hwnd);
        setupWorld();
        SDL_Event e;
        bool quit = false;
        double lastTime = clock();
        while ( !quit ) {
            while ( SDL_PollEvent(&e) ) {
                switch ( e.type ) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    switch ( e.window.event ) {
                    case SDL_WINDOWEVENT_RESIZED:
                        DirectX::XMINT2 newSize(e.window.data1, e.window.data2);
                        setRendererSize(newSize); // TODO not working
                        Camera::getInstance().setProjectionSize(newSize);
                        break;
                    }
                    break;
                case SDL_KEYDOWN:
                    if ( e.key.keysym.scancode == SDL_SCANCODE_S ) {
                        Camera::getInstance().move(BACKWARD, 0.2f);
                    } else if ( e.key.keysym.scancode == SDL_SCANCODE_W ) {
                        Camera::getInstance().move(FORWARD, 0.2f);
                    } else if ( e.key.keysym.scancode == SDL_SCANCODE_A ) {
                        Camera::getInstance().move(LEFT, 0.2f);
                    } else if ( e.key.keysym.scancode == SDL_SCANCODE_D ) {
                        Camera::getInstance().move(RIGHT, 0.2f);
                    } else if ( e.key.keysym.scancode == SDL_SCANCODE_ESCAPE ) {
                        quit = true;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    //Camera::getInstance().setTarget(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
                    break;
                case SDL_MOUSEMOTION:
                    float xrel = (float)e.motion.xrel;
                    Camera::getInstance().rotate(YAW, (float)e.motion.xrel);
                    Camera::getInstance().rotate(PITCH, -(float)e.motion.yrel);
                    break;
                }
            }
            renderFrame(16.6667f);
            double now = clock();
            int rest = (int)(16.6667f - (now - lastTime));
            //double rest = 1000.0f - (now - lastTime);
            //cout << "Elapsed time: " << now - lastTime << "   Rest: " << rest << endl;
            if ( rest > 0 ) {
                Sleep(rest);
            }
            lastTime = clock();
        }
    }
    catch ( string e ) {
        cout << "Caught exception: " << e << endl;
    }
}
void AbstractGame::cleanD3D()
{
    // close and release all existing COM objects
    if (swapchain != nullptr)
        swapchain->Release();
    if (dev != nullptr)
        dev->Release();
    if (backbuffer != nullptr)
        backbuffer->Release();
    if (devcon != nullptr)
        devcon->Release();
    
}









