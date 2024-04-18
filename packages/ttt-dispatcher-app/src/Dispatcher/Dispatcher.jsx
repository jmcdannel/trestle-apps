import React from 'react';

import Grid from '@mui/material/Grid';
import Box from '@mui/material/Box';

import DispatcherMenu from './DispatcherMenu';
import Route from '../Routes/Route';
import RouteMap from '../Routes/RouteMap';
import Turnout from '../Turnouts/Turnout';
import useLayoutRoute from '../Routes/useLayoutRoute';
import { useTurnoutStore } from '../Store/useTurnoutStore';
import { useTurnout } from '../Turnouts/useTurnout';

import './Dispatcher.scss';

const sleep = ms => new Promise(r => setTimeout(r, ms));

const TURNOUT_DELAY = 10; // ms

export const Dispatcher = props => {

  const {
    filter, 
    enabled, 
    overrideUserPrefs
  } = props;

  const { updateTurnout } = useTurnout();
  const turnouts = useTurnoutStore(state => state.turnouts);
  const dispatcherLayout = { map: true, routes: true, turnouts: true };
  const {
    computedRoutes,
    handleRouteToggle,
  } = useLayoutRoute();

  const setTurnouts = async deltas => {
    for(const delta of deltas){
      handleTurnoutChange(delta);
      await sleep(TURNOUT_DELAY);
    }
  }

  const handleTurnoutChange = async delta => {
    try {
      const t = turnouts.find(t => t.turnoutId === delta.turnoutId)
      console.log('[ROUTE ENGINE] handleTurnoutChange', t.turnoutId, delta)
      updateTurnout({...t, state: delta.state})
    } catch (err) {
      console.error(err);
      // throw err;
    }   
  }

  const isVisible = (item) => overrideUserPrefs
    ? enabled.includes(item)
    : enabled.includes(item) || !!dispatcherLayout[item];
  
  return turnouts ? (
    <Box sx={{ 
      alignContent: 'flex-start',
      overflow:'auto',
      flex: '1'
      }}>
      {isVisible('menu') && (
        <DispatcherMenu setTurnouts={setTurnouts} dispatcherLayout={dispatcherLayout}  />
      )}

      {isVisible('map') && (
        <RouteMap setTurnouts={setTurnouts} handleTurnoutChange={handleTurnoutChange} />
      )}

      {isVisible('routes') && (
        <Box className="routes">
          {computedRoutes().map(rte => (
            <Box 
              key={rte.routeId} 
              sx={{
                padding: '.25rem',
                flex: {
                  xs: '1 1 100%',
                  sm: '1 1 50%',
                  md: '1 1 33%',
                  lg: '1 1 25%',
                  xl: '1 1 20%',
                },
                maxWidth: {
                  xs: '100%',
                  sm: '50%',
                  md: '33%',
                  lg: '25%',
                  xl: '20%',
              }
            }}>
              <Route 
                className={rte.className} 
                route={rte} 
                handleRouteToggle={handleRouteToggle} 
              />
            </Box>
          ))}
        </Box>
      )}
      
      {isVisible('turnouts') && (
        <Box className="turnouts">        
        {turnouts?.filter(filter).map(turnout => (
            <Box 
              key={`turnout$${turnout.turnoutId}`} 
              sx={{
                padding: '.25rem',
                flex: {
                  xs: '1 1 100%',
                  sm: '1 1 50%',
                  md: '1 1 33%',
                  lg: '1 1 25%',
                  xl: '1 1 20%',
                },
                maxWidth: {
                  xs: '100%',
                  sm: '50%',
                  md: '33%',
                  lg: '25%',
                  xl: '20%',
                }
              }}>
              <Turnout turnout={turnout} handleTurnoutChange={handleTurnoutChange} />
            </Box>
          ))}            
        </Box>
      )}
    </Box>
  ) : null;

}

Dispatcher.defaultProps = {
  filter: turnouts => turnouts,
  enabled: ['menu'],
  overrideUserPrefs: false
};

export default Dispatcher;