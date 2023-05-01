// #include <wx/image.h>
// #include <wx/statbmp.h>
// #include <wx/wx.h>

// //
// #include <Windows.h>

// class MyFrame : public wxFrame {
// public:
//     MyFrame()
//         : wxFrame(nullptr, wxID_ANY, "wxWidgets Example", wxDefaultPosition, wxSize(300, 400)) {
//         SetBackgroundColour(*wxBLACK);

//         // wxMenu* helpMenu = new wxMenu;
//         // helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");

//         // wxMenuBar* menuBar = new wxMenuBar;
//         // menuBar->Append(helpMenu, "&Help");
//         // menuBar->SetBackgroundColour(*wxBLACK);
//         // menuBar->SetForegroundColour(*wxWHITE);
//         // SetMenuBar(menuBar);

//         wxPanel* panel = new wxPanel(this);
//         panel->SetBackgroundColour(wxColour(255, 0, 0));  // set background color to red

//         wxBoxSizer* sizer1 = new wxBoxSizer(wxHORIZONTAL);
//         panel->SetSizer(sizer1);

//         wxMenu* fileMenu = new wxMenu();
//         fileMenu->Append(wxID_OPEN, wxT("&Open"));
//         fileMenu->Append(wxID_SAVE, wxT("&Save"));
//         fileMenu->AppendSeparator();
//         fileMenu->Append(wxID_EXIT, wxT("&Quit"));

//         wxMenu* helpMenu = new wxMenu();
//         helpMenu->Append(wxID_ABOUT, wxT("&About"));

//         sizer1->Add(fileMenu, 0, wxALIGN_LEFT);
//         sizer1->Add(helpMenu, 0, wxALIGN_LEFT);
//         sizer1.add

//         wxMenuBar* menuBar = new wxMenuBar();
//         menuBar->SetSizer(sizer1);

//         SetMenuBar(menuBar);

//         // // //

//         Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);

//         wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

//         wxImage::AddHandler(new wxPNGHandler);
//         wxImage         image("Logo.png", wxBITMAP_TYPE_PNG);
//         wxStaticBitmap* bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(image));
//         sizer->Add(bitmap, 0, wxALL | wxALIGN_CENTER, 10);

//         checkList = new wxCheckListBox(this, wxID_ANY);
//         checkList->Append("a text");
//         checkList->Append("b text");
//         checkList->Append("c text");
//         sizer->Add(checkList, 0, wxALL | wxEXPAND, 10);

//         wxButton* goButton = new wxButton(this, wxID_ANY, "Go!");
//         sizer->Add(goButton, 0, wxALL | wxALIGN_CENTER, 10);

//         Bind(wxEVT_BUTTON, &MyFrame::OnGo, this, goButton->GetId());

//         SetSizerAndFit(sizer);
//     }

// private:
//     void OnAbout(wxCommandEvent& WXUNUSED(event)) {
//         wxMessageBox(
//             "This is a wxWidgets example program", "About", wxOK | wxICON_INFORMATION, this
//         );
//     }
//     void OnGo(wxCommandEvent& WXUNUSED(event)) {
//         wxString selectedItems;
//         for (size_t i = 0; i < checkList->GetCount(); ++i) {
//             if (checkList->IsChecked(i)) {
//                 selectedItems += checkList->GetString(i) + "\n";
//             }
//         }
//         if (!selectedItems.IsEmpty()) {
//             wxMessageBox(selectedItems, "Selected Items", wxOK | wxICON_INFORMATION, this);
//         } else {
//             wxMessageBox("No items selected.", "Selected Items", wxOK | wxICON_WARNING, this);
//         }
//     }
//     wxCheckListBox* checkList;
// };

// class MyApp : public wxApp {
// public:
//     bool OnInit() {
//         MyFrame* frame = new MyFrame();
//         frame->Show(true);
//         return true;
//     }
// };

// int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
//     auto* app = new MyApp;
//     wxApp::SetInstance(app);

//     int    argc = 0;
//     char** argv = nullptr;
//     if (wxEntryStart(argc, argv)) {
//         if (wxTheApp->OnInit()) wxTheApp->OnRun();
//         wxTheApp->OnExit();
//         wxEntryCleanup();
//     }

//     return 0;
// }

#include <wx/wx.h>

// class NoPaddingButton : public wxButton {
// public:
//     NoPaddingButton(
//         wxWindow* parent, wxWindowID id, const wxString& label = wxEmptyString,
//         const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style =
//         0, const wxValidator& validator = wxDefaultValidator, const wxString& name =
//         wxButtonNameStr
//     )
//         : wxButton(parent, id, label, pos, size, style, validator, name) {}

// protected:
//     wxSize DoGetBestSize() const override {
//         wxSize textSize = GetTextExtent(GetLabel());
//         textSize.x -= 5;  // Add some extra width for better appearance
//         textSize.y += 0;  // Add some extra height for better appearance
//         return textSize;
//     }
// };

class CustomButton : public wxPanel {
public:
    CustomButton(wxWindow* parent, const wxString& label)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL) {
        SetForegroundColour(*wxWHITE);
        SetBackgroundColour(*wxBLACK);
        m_label = new wxStaticText(this, wxID_ANY, label);
        m_label->Bind(wxEVT_LEFT_DOWN, &CustomButton::OnLeftDown, this);

        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(m_label, 0, wxALIGN_CENTER_VERTICAL);
        SetSizer(sizer);

        SetMinSize(DoGetBestSize());
        Bind(wxEVT_LEFT_DOWN, &CustomButton::OnLeftDown, this);
    }

protected:
    wxSize DoGetBestSize() const {
        wxSize textSize = m_label->GetTextExtent(m_label->GetLabel());
        textSize.x += 10;  // Add some extra width for better appearance
        textSize.y += 6;   // Add some extra height for better appearance
        return textSize;
    }

    void OnLeftDown(wxMouseEvent& event) {
        // Process button click event
        wxCommandEvent buttonEvent(wxEVT_BUTTON, GetId());
        buttonEvent.SetEventObject(this);
        GetEventHandler()->ProcessEvent(buttonEvent);
    }

private:
    wxStaticText* m_label;
};

class CustomMenuItem : public wxPanel {
public:
    CustomMenuItem(wxWindow* parent, const wxString& label, wxMenu* menu)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL) {
        SetBackgroundColour(*wxBLACK);

        wxBoxSizer* sizer  = new wxBoxSizer(wxHORIZONTAL);
        auto*       button = new CustomButton(this, label);
        sizer->Add(button);
        // sizer->SetSizeHints(this);
        // sizer->SetDimension(0, 0, sizer->GetMinSize().GetWidth(),
        // sizer->GetMinSize().GetHeight());
        SetSizer(sizer);

        m_menu = menu;

        button->Bind(wxEVT_BUTTON, &CustomMenuItem::OnButtonClick, this);
    }

private:
    void OnButtonClick(wxCommandEvent& event) {
        wxButton* button    = static_cast<wxButton*>(event.GetEventObject());
        wxPoint   buttonPos = button->GetPosition();
        wxPoint   menuPos(buttonPos.x, buttonPos.y + button->GetSize().y);
        PopupMenu(m_menu, menuPos);
    }

    wxMenu* m_menu;
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() override;
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);
};

bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("Custom Menu Bar");
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)) {
    SetBackgroundColour(*wxBLACK);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxPanel* customMenuBar =
        new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    customMenuBar->SetBackgroundColour(*wxBLACK);

    wxBoxSizer* menuBarSizer = new wxBoxSizer(wxHORIZONTAL);
    menuBarSizer->AddSpacer(12);

    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_OPEN, wxT("&Open"));
    fileMenu->Append(wxID_SAVE, wxT("&Save"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, wxT("&Quit"));

    CustomMenuItem* fileMenuItem = new CustomMenuItem(customMenuBar, "File", fileMenu);
    menuBarSizer->Add(fileMenuItem);

    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, "&About");

    CustomMenuItem* helpMenuItem = new CustomMenuItem(customMenuBar, "Help", helpMenu);
    menuBarSizer->Add(helpMenuItem);

    customMenuBar->SetSizer(menuBarSizer);
    mainSizer->Add(customMenuBar, 0, wxEXPAND);

    SetSizer(mainSizer);

    Bind(
        wxEVT_MENU,
        [](wxCommandEvent& event) {
            wxMessageBox("This is a custom menu bar example", "About", wxOK | wxICON_INFORMATION);
        },
        wxID_ABOUT
    );
}

wxIMPLEMENT_APP(MyApp);