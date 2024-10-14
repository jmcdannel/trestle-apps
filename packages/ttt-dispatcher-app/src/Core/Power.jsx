import React, { useState, useEffect } from 'react';
import Box from '@mui/material/Box';
import Backdrop from '@mui/material/Backdrop';
import SpeedDial from '@mui/material/SpeedDial';
import SpeedDialAction from '@mui/material/SpeedDialAction';
import PowerSettingsNewIcon from '@mui/icons-material/PowerSettingsNew';
import { StyledEngineProvider } from '@mui/material/styles';

import { useConnectionStore, CONNECTION_STATUS } from '../Store/useConnectionStore';
import { useDccStore } from '../Store/useDccStore';
import { useEffectStore } from '../Store/useEffectStore';
import useLayoutEffect from '../Effects/useLayoutEffect';
import { useDcc } from '../Dcc/useDcc'
import { useMqtt } from '../Core/Com/MqttProvider'

import './Power.scss'

const specialEfx = [
  {
    name: 'Lights',
    effectId: 201,
  },
  {
    name: 'Layout',
    effectId: 202,
  },
  {
    name: 'Track',
    effectId: 203,
  },
  {
    name: 'ALL',
    effectId: 204,
  }
]

export default function SpeedDialTooltipOpen() {
  const [open, setOpen] = useState(false);
  const handleOpen = () => setOpen(true);
  const handleClose = () => setOpen(false);

  const track = 'MAIN';
  const { dcc } = useMqtt();
  const { setPower } = useDcc()
  const { updateEffect } = useLayoutEffect();
  const effects = useEffectStore(state => state.effects);
  const powerEfx = effects.filter(efx => efx.type === 'power')
  const dccDeviceStatus = useConnectionStore(state => state.dccDeviceStatus);
  const powerStatus = useDccStore(state => state.powerStatus);
  const setPowerStatus = useDccStore(state => state.setPowerStatus);
  // const [ powerStatus, setPowerStatus ] = useState(undefined);
  const connected = dccDeviceStatus === CONNECTION_STATUS.CONNECTED;


  useEffect(async () => {
    if (typeof powerStatus !== 'undefined') {
      setPower(powerStatus, track);
    }
  }, [powerStatus]);

  const handlePowerClick = async () => {
    setPowerStatus(!powerStatus);
  }

  const handleEfxClick = async (efx) => {
    try {
      console.log('[Effect] updateEffect');
      await updateEffect({ ...efx, state: !efx.state });
    } catch (err) {
      console.error(err);
    }
  }

  const getCurrentStateKey = (state) => {
    if (typeof state === 'undefined') {
      return 'unknown';
    } else if (state === true) {
      return 'on';
    } else if (state === false) {
      return 'off';
    } else {
      return 'unknown';
    }
  }

  const overallPowerState = () => {
    return powerStatus && powerEfx.every(efx => efx.state);
  }

  const getClassName = () => `power-${getCurrentStateKey()}`;


  return (
    <StyledEngineProvider injectFirst>
      <Box sx={{ width: '4rem' }}></Box>
      <SpeedDial
        ariaLabel="SpeedDial tooltip example"
        sx={{ position: 'absolute', top: 16, right: 16 }}
        icon={<PowerSettingsNewIcon className={`power-${getCurrentStateKey(overallPowerState())}`} />}
        onClose={handleClose}
        onOpen={handleOpen}
        open={open}
        direction="down"
        className={`main-power-${getCurrentStateKey(overallPowerState())}`}
      >
        <SpeedDialAction
          icon={<PowerSettingsNewIcon />}
          tooltipTitle={'Track'}
          tooltipOpen
          className={`power-${getCurrentStateKey(powerStatus)}`}
          onClick={handlePowerClick}
        />
        {powerEfx.map((efx) => (
          <SpeedDialAction
            key={efx.effectId}
            icon={<PowerSettingsNewIcon />}
            tooltipTitle={efx.name}
            tooltipOpen
            className={`power-${getCurrentStateKey(!!efx.state)}`}
            onClick={() => handleEfxClick(efx)}
          />
        ))}
      </SpeedDial>
      <Backdrop open={open} />
    </StyledEngineProvider>
  );
}
