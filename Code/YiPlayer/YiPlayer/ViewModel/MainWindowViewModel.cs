using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace VideoToImagesConverter.ViewModel
{
    class MainWindowViewModel
    {
        public List<string> VideoFiles { set; get; }

        public MainWindowViewModel()
        {
            VideoFiles = new List<string> { };
        }
    }
}
