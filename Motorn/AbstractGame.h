#pragma once
#include <windows.h>
#include <d3d11.h>
#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_main.h"
#include "SDL_syswm.h"
#include <vector>
#include "D3Dcompiler.h"
#include <fstream>
#include <math.h>
#include <ctime>
#include "Vertex.h"
#include "Mesh.h"
#include "Entity.h"
#include "Structs.h"
#include "PointLightComponent.h"
class AbstractGame
{
private:
    SDL_Window *win;
    SDL_Renderer *ren;
    SDL_Surface *bmp;
    SDL_Texture *tex;
    IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
    ID3D11Device *dev;                     // the pointer to our Direct3D device interface
    ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
    ID3D11RenderTargetView *backbuffer;
    ID3D11Texture2D* depthStencilBuffer;
    ID3D11DepthStencilState* depthStencilState;
    ID3D11DepthStencilView* depthStencilView;
    ID3D11BlendState* blendState;
    HWND hwnd;
    int mouseX;
    int mouseY;
protected:
    virtual void                        setupWorld() = 0;
    virtual void                        update(const double delta) = 0;
    DirectX::XMFLOAT4X4                 mWorldTransform;
    Entity*                             mWorld;
    std::vector<Entity*>                mRayCastableEntities;
    std::vector<PointLightComponent*>   mPointLights;
    DirectionalLight*                   mDirectionalLight;
    D3dStuff                            mStuff;
public:
    // function prototypes
    void                initD3D(HWND hWnd);    
    void                cleanD3D(void);        
    bool                initSDL();
    bool                pipelineInit();
    void                startGame();
    void                renderFrame(const double delta);
    void                setFrameConstants(const PerFrameBuffer &constants);
    DirectX::XMINT2        getWindowSize();
    void                setRendererSize(const DirectX::XMINT2 &size);
                        AbstractGame();
    virtual                ~AbstractGame();
};

