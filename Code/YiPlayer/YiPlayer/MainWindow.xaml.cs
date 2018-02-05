using Microsoft.Win32;
using System.Windows;
using System.Windows.Input;
using Video2Photos;
using VideoToImagesConverter.ViewModel;

namespace VideoToImagesConverter
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        MainWindowViewModel viewModel = new MainWindowViewModel();
        public MainWindow()
        {
            InitializeComponent();
            DataContext = viewModel;
        }

        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }

        private void CloseBtn_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void AddFileButton_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.OpenFileDialog dlg = new System.Windows.Forms.OpenFileDialog();
            dlg.Filter = "MP4 (*.mp4)|*.mp4";
            dlg.Multiselect = false;
            if (dlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                foreach (var item in dlg.FileNames)
                {
                    viewModel.VideoFiles.Add(item);
                }
            }


        }
    }
}
