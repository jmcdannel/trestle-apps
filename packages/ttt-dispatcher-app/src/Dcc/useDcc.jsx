import { useEffect } from 'react'
import { getAuth, onAuthStateChanged } from 'firebase/auth';
import { useMqtt } from '../Core/Com/MqttProvider'
import { useDejaCloud } from '../Core/Com/useDejaCloud'
import { firebaseApp } from '../firebase';

export function useDcc() {
  const auth = getAuth(firebaseApp);
  const { dcc, isConnected } = useMqtt();
  const { send: dejaCloudSend } = useDejaCloud();

  async function setPower(payload, track = 'MAIN') {
    try {
      if (typeof payload === 'undefined') {
        console.log('[DCC API].setPower', 'payload undefined');
        return;
      }
      console.log('[DCC API].setPower', payload);
      send('power', `${payload ? 1 : 0} ${track}`);
    } catch (err) {
      console.error('[DCC API].setPower', err);
      throw new Error('Unable to read', err);
    }
  }

  async function setSpeed(address, speed) {
    try {
      send('throttle', { address, speed });
    } catch (err) {
      console.error('[DCC API].setSpeed', err);
      throw new Error('Unable to read', err);
    }
  }

  async function setTurnout(turnoutId, state) {
    try {
      send('turnout', { turnoutId, state });
    } catch (err) {
      console.error('[DCC API].setTurnout', err);
      throw new Error('Unable to read', err);
    }
  }

  async function setOutput(pin, state) {
    try {
      send('output', { pin, state });
    } catch (err) {
      console.error('[DCC API].setTurnout', err);
      throw new Error('Unable to read', err);
    }
  }

  async function setFunction(address, func, state) {
    try {
      send('function', { address, func, state });
    } catch (err) {
      console.error('[DCC API].setPower', err);
      throw new Error('Unable to read', err);
    }
  }

  async function send(action, payload) {
    try {
      if (auth?.currentUser) {
        dejaCloudSend({ action, payload });
      } else if (isConnected) {
        dcc(action, payload)
        console.log('DCC send', action, payload)
      } else {
        throw new Error('Not connected');
      }
    } catch (err) {
      console.error('[DCC API].send', err, action, payload)
    }
  }

  useEffect(async () => {

  }, [])


  return {
    send,
    setOutput,
    setPower,
    setSpeed,
    setTurnout,
    setFunction,
  }
}

export default useDcc
