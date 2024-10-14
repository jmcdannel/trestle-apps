import React, { useEffect, useState } from 'react';
import { Route, Routes } from "react-router-dom";
import { getAuth, onAuthStateChanged } from 'firebase/auth';
import Login from '../Core/Login';
import Conductor from '../Conductor/Conductor';
import Pinout from '../Settings/Pinout'; // TODO: refactor
import Settings from '../Core/Settings';
import ApiEngine from '../Core/Com/ApiEngine';
import CloudEngine from '../Core/Com/CloudEngine';
import { Dashboard as DccDashboard } from '../Dcc/Dashboard';
import Dispatcher from '../Dispatcher/Dispatcher';
import Effects from '../Effects/Effects';
import { useConnectionStore } from '../Store/useConnectionStore';
import { useEffectStore } from '../Store/useEffectStore';
import { useLocoStore } from '../Store/useLocoStore';
import { useLayoutStore } from '../Store/useLayoutStore';
import { useRouteStore } from '../Store/useRouteStore';
import { useTurnoutStore } from '../Store/useTurnoutStore';
import { firebaseApp } from '../firebase';

function Modules(props) {


  const isGuest = useConnectionStore(state => state.isGuest);
  const setIsGuest = useConnectionStore(state => state.setIsGuest);
  const layoutId = useConnectionStore(state => state.layoutId);
  const auth = getAuth(firebaseApp)

  const [isAuthenticated, setIsAuthenticated] = useState(false)

  useEffect(() => {
    onAuthStateChanged(auth, async function (user) {
      if (user) {
        // User is signed in.
        console.log('User is signed in.', auth)
        setIsAuthenticated(true)
        // await dejaJsApi.connectDejaCloud()
      } else {
        // No user is signed in.
        console.log('No user is signed in.', auth)
        // await dejaJsApi.connectMqtt()
      }
    }), [auth]
  })

  const locos = useLocoStore(state => state.locos);
  const turnouts = useTurnoutStore(state => state.turnouts);
  const routes = useRouteStore(state => state.routes);
  const effects = useEffectStore(state => state.effects);
  const layout = useLayoutStore(state => state.layout);
  const loading = (<div>Loading</div>);

  const getRoutedModule = module => {
    switch (module) {
      case 'turnouts':
        return (
          <Route path="/dispatcher" key={module} element={
            (routes || turnouts) && <Dispatcher />
          } />
        );
      case 'effects':
        return (
          <Route path="/effects" key={module} element={
            effects && <Effects />
          } />
        )
      default:
        return null;
    }
  }
  console.log('[Modules] isAuthenticated', isAuthenticated, isGuest)
  return (isAuthenticated || isGuest)
    ? (<>
      {/* <pre onClick={() => setIsGuest(false)}>isGuest: {isGuest?.toString()}</pre>
      <pre style={{ textAlign: 'left' }}>isAuth: {(!!auth?.currentUser).toString()}</pre> */}
      {isGuest && <ApiEngine />}
      {(!!auth?.currentUser && layoutId) && <CloudEngine />}
      <Routes>
        <Route path="/" exact element={locos ? <Conductor /> : loading} />
        <Route path="/pinout" exact element={<Pinout />} />
        <Route path="/settings" exact element={<Settings />} />
        <Route path="/dcc" exact element={<DccDashboard />} />
        {layout?.modules && layout.modules.map(getRoutedModule)}
      </Routes>
    </>) : (<Login />)
}

export default Modules;