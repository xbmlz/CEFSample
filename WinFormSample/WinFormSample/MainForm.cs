using CefSharp;
using CefSharp.WinForms;
using System.Windows.Forms;

namespace WinFormSample
{
    public partial class MainForm : Form
    {
        public ChromiumWebBrowser browser;

        public MainForm()
        {
            InitializeComponent();
        }


        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 窗体关闭时退出CEF
            browser.Dispose();
            browser = null;
            Cef.Shutdown();
        }

        private void MainForm_Load(object sender, System.EventArgs e)
        {
            InitializeBrowser();
            WindowState = FormWindowState.Maximized;
        }

        // 初始化CEF
        public void InitializeBrowser()
        {
            var settings = new CefSettings
            {
                Locale = "zh-CN"
            };
            Cef.EnableHighDPISupport();
            Cef.Initialize(settings);
            browser = new ChromiumWebBrowser("https://www.baidu.com/")
            {
                // 填充整个父组件
                Dock = DockStyle.Fill,
                // 按键响应
                KeyboardHandler = new CefKeyBoardHander()
            };
            Controls.Add(browser);
        }
    }
}
