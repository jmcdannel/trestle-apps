import React, { useEffect, useRef, useState } from 'react';
import Box from '@mui/material/Box';
import Card from '@mui/material/Card';
import CardContent from '@mui/material/CardContent';
import Chip from '@mui/material/Chip';
import Typography from '@mui/material/Typography';
import UsbIcon from '@mui/icons-material/Usb';
import UsbOffIcon from '@mui/icons-material/UsbOff';
import RouterIcon from '@mui/icons-material/Router';
import Skeleton from '@mui/material/Skeleton';
import Stack from '@mui/material/Stack';
import UsbOutlinedIcon from '@mui/icons-material/UsbOutlined';
import UsbOffOutlinedIcon from '@mui/icons-material/UsbOffOutlined';
import { useDejaCloud } from '../Core/Com/useDejaCloud';
import { DccDeviceDialog } from './DccDeviceDialog';
import { useConnectionStore, CONNECTION_STATUS } from '../Store/useConnectionStore';

export const DejaCloud = () => {

  const { getLayout } = useDejaCloud();
  const [layout, setLayout] = useState(null);
  const [deviceOpen, setDeviceOpen] = useState(false);
  const dccDevice = useConnectionStore(state => state.dccDevice);
  const dccDeviceStatus = useConnectionStore(state => state.dccDeviceStatus);
  const setDccDevice = useConnectionStore(state => state.setDccDevice);

  const deviceConnected = dccDeviceStatus === CONNECTION_STATUS.CONNECTED;

  useEffect(() => {
    async function fetchData() {
      const layout = await getLayout();
      console.log('layout', layout);
      setLayout(layout)
    }
    fetchData();
  }, []);

  return (
    <div>
      <h1>DejaCloud</h1>
      <Card className="connection">
        <CardContent sx={{
          alignItems: 'center',
          justifyContent: 'center',
          display: 'flex',
        }}>
          <Box sx={{ padding: '1rem' }}>
            {deviceConnected
              ? <UsbOutlinedIcon sx={{ fill: 'green', fontSize: '8rem' }} />
              : <UsbOffOutlinedIcon sx={{ fill: 'red', fontSize: '8rem' }} />}
          </Box>
          <Stack spacing={1} sx={{ padding: '1rem', flex: '1' }}>

            <Typography>DCC-EC Command Station: </Typography>
            <Chip
              onClick={() => setDeviceOpen(true)}
              sx={{ justifyContent: 'space-between' }}
              icon={deviceConnected
                ? <UsbIcon
                  sx={{ paddingLeft: '.5rem' }}
                  className={`status--${dccDeviceStatus}`} />
                : <UsbOffIcon
                  sx={{ paddingLeft: '.5rem' }}
                  className={`status--${dccDeviceStatus}`} />
              }
              label={dccDevice ? dccDevice : <Skeleton width={150} />}
              onDelete={() => setDccDevice(null)}
            />
          </Stack>
        </CardContent>
      </Card>
      <DccDeviceDialog
        onClose={() => setDeviceOpen(false)}
        open={deviceOpen}
        ports={layout?.ports}
      />
      <pre style={{ textAlign: 'left' }}>{JSON.stringify(layout || {}, null, 2)}</pre>
    </div>
  );
}

export default DejaCloud;