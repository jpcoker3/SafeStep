namespace TabbedPageSample;

public partial class App : Application
{
	public App()
	{
		InitializeComponent();

        // Force light theme
        UserAppTheme = AppTheme.Light;
        
		MainPage = new AppTabbedPage();
	}
}