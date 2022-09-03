using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UI.View;
using UI;
using System.Threading; //// Add this in your file
using System.Runtime.InteropServices;
using System.Windows.Interop;
using System.Windows.Media.Media3D;

class UIBridge
{

    public static MainView mainview_ui;
    public static Thread gui_thread;
    public static IntPtr mainview_handle = IntPtr.Zero;


    [DllExport]
    static public IntPtr CreateUserInterface
           (uint parentthread) /// Multi-Threaded Version
    {

        gui_thread = new Thread(() =>
        {
            mainview_ui = new MainView(parentthread)
            { Opacity = 1, Width = 1280, Height = 720 };
            mainview_ui.Show();
            mainview_handle = new WindowInteropHelper(mainview_ui).Handle;
            System.Windows.Threading.Dispatcher.Run();

            mainview_ui.Hide();
            mainview_ui.Opacity = 1;
        });
        gui_thread.SetApartmentState(ApartmentState.STA); /// STA Thread Initialization
        gui_thread.Start();

        while (mainview_handle == IntPtr.Zero) { }
        return mainview_handle;
    }

    //[DllExport]
    //static public int GetGuiThread()
    //{
    //    return gui_thread.ManagedThreadId; 
    //}
    [DllExport]
    static public void DisplayUserInterface() /// Multi-Threaded Version
    {
        try
        {

            mainview_ui.Show();
            mainview_ui.ShowInTaskbar = true;
            //mainview_ui.Opacity = 1;
            //mainview_ui.Width = 1280 ;
            //mainview_ui.Height = 720;
    }
        catch /// Can't Access to UI Thread, So Dispatching
        {
            mainview_ui.Dispatcher.BeginInvoke((Action)(() => {

                mainview_ui.Show();
                mainview_ui.ShowInTaskbar = true;
            }));
        }
    }

    [DllExport]
    static public void DestroyUserInterface() /// Multi-Threaded Version
    {
        try
        {
            mainview_ui.Close();
        }
        catch /// Can't Access to UI Thread, So Dispatching
        {
            mainview_ui.Dispatcher.BeginInvoke((Action)(() => {
                mainview_ui.Close();
            }));
        }
    }
    [DllExport]
    static public void HideUserInterface() /// Multi-Threaded Version
    {
        try
        {
            mainview_ui.Hide();
            mainview_ui.ShowInTaskbar = false;
        }
        catch /// Can't Access to UI Thread, So Dispatching
        {
            mainview_ui.Dispatcher.BeginInvoke((Action)(() => {
                mainview_ui.Hide();
            }));
        }
    }

    [DllExport]
    static public void UpdateProgress() /// Multi-Threaded Version
    {
        try
        {
            mainview_ui.UpdateStatus();
        }
        catch /// Can't Access to UI Thread, So Dispatching
        {
            mainview_ui.Dispatcher.BeginInvoke((Action)(() => {
                mainview_ui.UpdateStatus();
            }));
        }
    }


}