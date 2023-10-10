namespace TabbedPageSample;

public partial class StatusPage : ContentPage
{
	public StatusPage()
	{
		InitializeComponent();
	}

    protected override void OnAppearing()
    {
        base.OnAppearing();

        // Get the current battery level and update the progress bar
        var batteryReading = 0.88;
        batteryProgressBar.Progress = batteryReading; // between 0 and 1
        batteryProgressLabel.Text = "Battery Level: " + batteryReading*100 + "%"; // copy the data from progress bar
        //control battery color
        if (batteryReading > 0.75)
        {
            batteryProgressBar.ProgressColor = Colors.Green;
        }else if (batteryReading > 0.25)
        {
            batteryProgressBar.ProgressColor = Colors.Orange;
        }
        else
        {
            batteryProgressBar.ProgressColor = Colors.Red;
        }





        //get current decibel reading
        var decibelReading = 75;
        //Decibel reading betweel 60 -120 dB. 
        var decibelProgress = Math.Round(((float)decibelReading - 20) / 120, 2);

        decibelProgressBar.Progress = (double)decibelProgress; // 120 is max decibels. 
        decibelLabel.Text = "Decibel Level: " + decibelReading + " db"; // describe current decibels

        if (decibelProgress >= 0.85)
        {
            decibelProgressBar.ProgressColor = Colors.Red;
        }
        else if (decibelProgress > 0.25)
        {
            decibelProgressBar.ProgressColor = Colors.Green;
        }
        else
        {
            decibelProgressBar.ProgressColor = Colors.Blue;
        }

        // Get current Temperature Rating
        var tempReading = 72;

        //we want a range from 65-75 on avg. we will expand to 55-85 for general purposes. 
        // number - 55: brings avg from 0 - 30. 
        //so (number -55)/30 should give us a range between 0-1 for the range 55-85.
        var tempProgress = Math.Round(((float)tempReading - 55) / 30, 2);
        temperatureProgressBar.Progress = (double)tempProgress;
        temperatureLabel.Text = "Temperature: " + tempReading + "°F";

        if (tempProgress > 0.75)
        {
            temperatureProgressBar.ProgressColor = Colors.Red;
        }
        else if (tempProgress > 0.25)
        {
            temperatureProgressBar.ProgressColor = Colors.Green;
        }
        else
        {
            temperatureProgressBar.ProgressColor = Colors.Red;
        }
    }
}


