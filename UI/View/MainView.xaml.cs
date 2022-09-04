using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using UI;
namespace UI.View
{

    /// <summary>
    /// Interaction logic for MainView.xaml
    /// </summary>
    public partial class MainView : Window
    {
        [DllImport("User32.dll", SetLastError = true, EntryPoint = "FindWindow")]
        public static extern IntPtr FindWindow(String lpClassName, String lpWindowName);

        [DllImport("User32.dll", SetLastError = true, EntryPoint = "SendMessage")]
        static extern long SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);
        [DllImport("User32.dll", SetLastError = true, EntryPoint = "SendMessage")]
        public static extern IntPtr SendMessageSIMPLE(IntPtr hWnd, int Msg, int wParam, int lParam);

        [return: MarshalAs(UnmanagedType.Bool)]
        [DllImport("user32.dll", SetLastError = true, EntryPoint = "PostThreadMessageA")]
        public static extern bool PostThreadMessage(uint threadId, uint msg, UIntPtr wParam, IntPtr lParam);


        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto, Pack = 1)]
        public struct COPYDATASTRUCT
        {
            public uint dwData;    // Any value the sender chooses.  Perhaps its main window handle?
            public int cbData;    // The count of bytes in the message.
            public IntPtr lpData;    // The address of the message.
        }

        public static IntPtr IntPtrAlloc<T>(T param)
        {
            IntPtr retval = Marshal.AllocHGlobal(Marshal.SizeOf(param));
            Marshal.StructureToPtr(param, retval, false);
            return (retval);
        }

        public static void IntPtrFree(IntPtr preAllocated)
        {
            if (IntPtr.Zero == preAllocated) throw (new Exception("Go Home"));
            Marshal.FreeHGlobal(preAllocated); preAllocated = IntPtr.Zero;
        }

        const uint WM_PRINT = 1025;
        const uint WM_SEND = 1026;
        public uint threadId;
        public uint threadIdMine;
        delegate int GetProgressUpdatePtr();
        delegate bool testsendptr(string filename, long filesize);
        public IntPtr parentwindowfind;
        public IntPtr parentwindowmine;
        public int amount;
        public int progress;
        GetProgressUpdatePtr GetProgressUpdate;
        public MainView(uint parentthread)
        {
            InitializeComponent();
            //GetProgressUpdate = (GetProgressUpdatePtr)Marshal.GetDelegateForFunctionPointer
            //        (apiptr, typeof(GetProgressUpdatePtr));
            //testsend = (testsendptr)Marshal.GetDelegateForFunctionPointer
            //                      (api_2_ptr, typeof(testsendptr));
            amount = 1000;
            //MyFindWindow();
            threadId = parentthread;
        }

        public void MyFindWindow()
        {

            IntPtr hwnd = FindWindow("CppMAppHostWinClass", "TEST");
            while (hwnd == IntPtr.Zero)
            {
                Thread.Sleep(5);
                hwnd = FindWindow("CppMAppHostWinClass", "TEST");
            }
            this.parentwindowfind = hwnd;
        }

        private void Print_Click(object sender, RoutedEventArgs e)
        {
            pbStatus.Value = 0;
            PostThreadMessage(threadId, WM_PRINT, UIntPtr.Zero, IntPtr.Zero);


            //Task.Factory.StartNew(() =>
            //{
            //    while (progress<= 100)
            //    {
            //        Dispatcher.Invoke(() =>
            //        {
            //            pbStatus.Value = progress;
            //            Thread.Sleep(10);
            //        });
            //    }
            //});
            // SendMessage(parentwindowfind, WM_PRINT, IntPtr.Zero, IntPtr.Zero);

        }
        public void UpdateStatus()
        {
            pbStatus.Value++;
        }
        private void Send_Click(object sender, RoutedEventArgs e)
        {

            IntPtr send = new IntPtr(amount);
            PostThreadMessage(threadId, WM_SEND, UIntPtr.Zero, send);
            //SendMessage(parentwindowfind, WM_SEND, IntPtr.Zero, send);
        }


    }
}

