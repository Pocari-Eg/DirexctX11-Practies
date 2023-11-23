#include "GUIClass.h"
#include"GraphicsClass.h"


GUIClass::GUIClass()
{
 
}

GUIClass::GUIClass(const GUIClass&)
{
}

GUIClass::~GUIClass()
{
}

bool GUIClass::Initialize(GraphicsClass* graphics ,HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext,
    ID3D11RenderTargetView* RenderTargetView)
{

    m_device = device;
    m_deviceContext = deviceContext;
    g_mainRenderTargetView = RenderTargetView;
    m_Graphics = graphics;
    m_hwnd = hwnd;


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    m_io = &io;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();


    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_hwnd);
    ImGui_ImplDX11_Init(m_device, m_deviceContext);


    // Our state
     return true;
}

void GUIClass::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void GUIClass::Draw()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).


    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;

        ImGui::Begin("Control");                          // Create a window called "Hello, world!" and append into it.
     
        // Display some text (you can use a format strings too)
         // Edit 1 float using a slider from 0.0f to 1.0f
        if (ImGui::SliderFloat("CameraZ", &f, 0.0f, 450.0f))
        {
            m_Graphics->CameraZoomIO(f);
        }

        ImGui::Text("Clear Color");
        if (ImGui::ColorEdit3("Clear", (float*)&Clear_color))
        {
            m_Graphics->SetClearColor(Clear_color.x, Clear_color.y, Clear_color.z, Clear_color.w);
        }
  
          ImGui::Text("Diffuse Color");
          if (ImGui::ColorEdit4("Diffuse", (float*)&Diffuse_color))
          {
              m_Graphics->SetDiffuseColor(Diffuse_color.x, Diffuse_color.y, Diffuse_color.z, Diffuse_color.w);
         }
   
        ImGui::Text("(%.1f FPS)", m_io->Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.

        //ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //ImGui::Text("Hello from another window!");
        //if (ImGui::Button("Close Me"))
        //    show_another_window = false;
        //ImGui::End();

        bool open = false, save = false;
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Model"))
            {
                if (ImGui::MenuItem("Open", NULL))
                    open = true;
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        //Remember the name to ImGui::OpenPopup() and showFileDialog() must be same...
        if (open)
            ImGui::OpenPopup("Open File");
        
 
        /* Optional third parameter. Support opening only compressed rar/zip files.
         * Opening any other file will show error, return false and won't close the dialog.
         */
        if (file_dialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(700, 310),".fbx,.obj,.3ds"))
        {
              std::cout<<"Load Model : "<< file_dialog.selected_fn << std::endl;      // The name of the selected file or directory in case of Select Directory dialog mode
               // The absolute path to the selected file


            std::string path ="../DirectX11/models/"+ file_dialog.selected_fn;
         

            m_Graphics->LoadModeldData(m_hwnd, path);
        }


    // Rendering
    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
