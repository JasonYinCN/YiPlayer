namespace VideoToImagesConverter.ViewModel
{
    using System;
    using System.ComponentModel;

    public abstract class ViewModelBase : INotifyPropertyChanged, IDisposable
    {
		public event PropertyChangedEventHandler PropertyChanged;

        public virtual void Dispose()
        {
            
        }

        protected virtual void RaisePropertyChanged(string propertyName)
		{
			PropertyChangedEventHandler handler = PropertyChanged;
			if(handler != null)
			{
				PropertyChangedEventArgs e = new PropertyChangedEventArgs(propertyName);
				handler(this, e);
			}
		}
	}
}
