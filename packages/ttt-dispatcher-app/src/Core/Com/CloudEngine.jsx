import React, { useContext, useEffect, useState } from 'react';
import { doc, onSnapshot } from "firebase/firestore";
import { useConnectionStore, CONNECTION_STATUS } from '../../Store/useConnectionStore';
import { useThrottleStore } from '../../Store/useThrottleStore';
import { useLocoStore } from '../../Store/useLocoStore';
import { useLayoutStore } from '../../Store/useLayoutStore';
import { useDejaCloud } from '../../Core/Com/useDejaCloud';
import { db } from '../../firebase';
import dayjs from 'dayjs'
import relativeTime from 'dayjs/plugin/relativeTime'

export function CloudEngine() {

  const { getLayout, send, init } = useDejaCloud();
  dayjs.extend(relativeTime);

  const [layout, setLayout] = useState(null);

  const layoutId = useConnectionStore(state => state.layoutId);
  const setStatus = useConnectionStore(state => state.setStatus);
  const dccDevice = useConnectionStore(state => state.dccDevice);
  const dccDeviceStatus = useConnectionStore(state => state.dccDeviceStatus);

  const locos = useLocoStore(state => state.locos);
  const initLayouts = useLayoutStore(state => state.initLayouts);


  const throttles = useThrottleStore(state => state.throttles);

  const setDccDevice = useConnectionStore(state => state.setDccDevice);
  const setDccDeviceStatus = useConnectionStore(state => state.setDccDeviceStatus);

  const upsertThrottle = useThrottleStore(state => state.upsertThrottle);

  const handleMessage = async (message) => {
    try {
      console.log('[CloudEngine] handleMessage', message);
      const { action, payload } = message.data;
      switch (action) {
        case 'portList':
          // setPorts(payload);
          break;
        case 'interfaces':
          // payload?.map(updateActionDeviceById)
          break;
        case 'connected':
          // console.log('[CloudEngine] connected', payload, dccDevice);
          // if (payload.serial === dccDevice) {
          //   setDccDeviceStatus(CONNECTION_STATUS.CONNECTED);
          // } else {
          //   updateActionDeviceStatusByPort(payload.serial, CONNECTION_STATUS.CONNECTED);
          // }
          break;
      }
    } catch (err) {
      // setDccDeviceStatus(CONNECTION_STATUS.DISCONNECTED);
      console.error('[CloudEngine] handleMessage error', err);
    }
  }

  const unsub = onSnapshot(doc(db, 'layouts', layoutId), (doc) => {
    if (dccDeviceStatus === CONNECTION_STATUS.CONNECTED) {
      return;
    }
    console.log("Current data: ", doc.data());
    const data = doc.data();
    if (data?.dccEx?.lastConnected?.seconds) {
      const now = dayjs()
      const connected = dayjs.unix(data.dccEx.lastConnected.seconds)
      if (now.diff(connected, 'minute') < 2) {
        console.log('layout lastConnected', data.dccEx.lastConnected.seconds, now.diff(connected, 'minute'), now.diff(connected, 'second'), now.diff(connected))
        setDccDeviceStatus(CONNECTION_STATUS.CONNECTED);
        setStatus(CONNECTION_STATUS.CONNECTED);
      }
    }
  });

  useEffect(() => {
    console.log('[CloudEngine] getLayout', layout);
    async function fetchData() {
      const layout = await getLayout();
      console.log('layout', layout);
      setLayout(layout)
      await send({
        action: 'connect',
        payload: { serial: dccDevice, dcc: true }
      });
      await init();
    }
    fetchData();
  }, []);

  // Connect DCC Device
  useEffect(() => {
    const initialize = async function () {
      console.log('[CloudEngine] Connect DCC Device', dccDeviceStatus, dccDevice);
      try {
        setDccDeviceStatus(CONNECTION_STATUS.PENDING);
        send({
          action: 'connect',
          payload: { serial: dccDevice, dcc: true }
        });
      } catch (err) {
        log.error('dcc device initialization error', err);
      }
    };
    if (dccDevice) {
      if (dccDeviceStatus === CONNECTION_STATUS.CONNECTED) {
        console.log('[CloudEngine] dccDeviceStatus', dccDeviceStatus);
        unsub();
      } else {
        initialize();
      }
    } else {
      setDccDeviceStatus(CONNECTION_STATUS.DISCONNECTED);
    }
    console.log('[CloudEngine] dccDevice', dccDevice, dccDeviceStatus);
  }, [dccDeviceStatus, dccDevice])

  return (<></>)

}

export default CloudEngine;