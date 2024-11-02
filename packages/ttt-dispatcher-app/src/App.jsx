import React, { useEffect, useState } from 'react';

import CssBaseline from '@mui/material/CssBaseline';
import { ThemeProvider as MuiThemeProvider } from '@mui/material/styles';
import { BrowserRouter } from "react-router-dom";

import theme from './theme';

import AppLayout from './Core/UI/AppLayout';
import Header from './Core/Header';
import Footer from './Core/Footer';
import Modules from './Core/Modules';
import MqttProvider from './Core/Com/MqttProvider';

import Store from './Store/Store';
import './App.scss';

function App() {

  return (
    <MuiThemeProvider theme={theme}>
      <Store>
        <MqttProvider>
          <CssBaseline />
          <BrowserRouter>
            <AppLayout
              header={<Header />}
              body={<Modules />}
              footer={<Footer />}
            />
          </BrowserRouter>
        </MqttProvider>
      </Store>
    </MuiThemeProvider>
  );
}

export default App;
