﻿
using Microsoft.Maui.Controls;
using System;
using System.IO;
using System.Text;
using System.Threading;
using InTheHand.Net.Bluetooth;
using InTheHand.Net.Sockets;

namespace TabbedPageSample;
public partial class StatusPage : ContentPage
{
    private BluetoothClient client;
	public StatusPage()
	{
		InitializeComponent();
        client = new BluetoothClient();

	}

    protected override void OnAppearing()
    {
        base.OnAppearing();
        setDecibel(75);
        setBatteryLevel(0.50);
        setTemperature(85);

        // Initiate Bluetooth connection
        ConnectBluetooth();



    }
    public void setDecibel(float decibelReading) // any int
    {

        //Decibel reading between 60 -120 dB. 
        var decibelProgress = Math.Round((float)decibelReading, 2);

        decibelProgressBar.Progress = (double)decibelProgress / 120; // 120 is max decibels. 
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

    }

    public void setTemperature(float tempReading) // any int
    {

        //we want a range from 65-75 on avg. we will expand to 55-85 for general purposes. 
        // number - 55: brings avg from 0 - 30. 
        //so (number -55)/30 should give us a range between 0-1 for the range 0 - 120.
        var tempProgress = Math.Round(((float)tempReading / 120), 2);
        temperatureProgressBar.Progress = (double)tempProgress;
        temperatureLabel.Text = "Heat Index: " + tempReading + "°F";

        if (tempProgress > 0.80)
        {
            temperatureProgressBar.ProgressColor = Colors.Red;
        }
        else if (tempProgress > 0.60)
        {
            temperatureProgressBar.ProgressColor = Colors.Green;
        }
        else
        {
            temperatureProgressBar.ProgressColor = Colors.Red;
        }

    }

    public void setBatteryLevel(double batteryReading) // 0-1
    {

        batteryProgressBar.Progress = batteryReading; // between 0 and 1
        batteryProgressLabel.Text = "Battery Level: " + batteryReading * 100 + "%"; // copy the data from progress bar
        //control battery color
        if (batteryReading > 0.75)
        {
            batteryProgressBar.ProgressColor = Colors.Green;
        }
        else if (batteryReading > 0.25)
        {
            batteryProgressBar.ProgressColor = Colors.Orange;
        }
        else
        {
            batteryProgressBar.ProgressColor = Colors.Red;
        }
    }

    private void ConnectBluetooth()
    {
        // Discover and connect to the Bluetooth device
        BluetoothDeviceInfo[] devices = (BluetoothDeviceInfo[])client.DiscoverDevices();

        BluetoothDeviceInfo device = null;

        foreach (BluetoothDeviceInfo obj in devices)
        {
            if (obj.DeviceName != null && obj.DeviceName.Contains("HC-05"))
            {
                // You can select the device based on its name, address, or any other criteria
                device = obj;
            }
        }


        if (device != null)
        {
            client.Connect(device.DeviceAddress, BluetoothService.SerialPort);

            // Start a new thread for data reception after connection
            Thread receiveThread = new Thread(ReceiveData);
            receiveThread.IsBackground = true;
            receiveThread.Start();
        }
        else
        {
            // Handle device not found or connection failure
        }
    }

    private void ReceiveData()
    {
        var stream = client.GetStream();
        byte[] receive = new byte[1024];

        while (true)
        {
            Array.Clear(receive, 0, receive.Length);
            var readMessage = "";
            do
            {
                
                int bytesRead = stream.Read(receive, 0, receive.Length);
                readMessage += Encoding.ASCII.GetString(receive, 0, bytesRead);
            }
            while (stream.DataAvailable);


            // at this point, the readMessage string will contain the entire message. 

            // Split the received message by commas to extract values
            string[] values = readMessage.Split(',');

            string humidity = "", decibel = "", latitude = "", longitude = "";
            bool fallen = false;

            foreach (string value in values)
            {
                if (value.StartsWith("hum:"))
                {
                    humidity = value.Substring(4);
                    System.Diagnostics.Debug.WriteLine($"Humidity: {humidity}");
                }
                else if (value.StartsWith("dec:"))
                {
                    decibel = value.Substring(4);
                    System.Diagnostics.Debug.WriteLine($"Decibel: {decibel}");
                }
                else if (value.StartsWith("coords:"))
                {
                    string[] coords = value.Substring(7).Split('x');
                    if (coords.Length == 2)
                    {
                        latitude = coords[0];
                        longitude = coords[1];
                        System.Diagnostics.Debug.WriteLine($"Latitude: {latitude}, Longitude: {longitude}");
                    }
                }
                else if (value.StartsWith("fall:true"))
                {
                    fallen = true;
                    System.Diagnostics.Debug.WriteLine("Fallen: true");
                }
                else if (value.StartsWith("fall:false"))
                {
                    fallen = false;
                    System.Diagnostics.Debug.WriteLine("Fallen: false");
                }
            }

        }
    }

}


