﻿using System;
using System.Net.Sockets;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace SafeStep
{
    public partial class App : Application
    {
        public App()
        {
            InitializeComponent();

            MainPage = new NavigationPage(new LocatePage());
        }

        protected override void OnStart()
        {
        }

        protected override void OnSleep()
        {
        }

        protected override void OnResume()
        {
        }


    }

}
