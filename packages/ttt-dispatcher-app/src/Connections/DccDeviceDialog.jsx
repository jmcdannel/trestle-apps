import React, { useContext, useState, useEffect } from 'react';
import Autocomplete from '@mui/material/Autocomplete';
import DialogTitle from '@mui/material/DialogTitle';
import TextField from '@mui/material/TextField';
import Dialog from '@mui/material/Dialog';
import SaveIcon from '@mui/icons-material/Save';
import Button from '@mui/material/Button';
import { useMqtt } from '../Core/Com/MqttProvider'
import { useConnectionStore } from '../Store/useConnectionStore';

export const DccDeviceDialog = ({ onClose, open, ports }) => {

  const setDccDevice = useConnectionStore(state => state.setDccDevice);
  const layoutId = useConnectionStore(state => state.layoutId);
  const { publish, isConnected } = useMqtt();
  const [newDccDevice, setNewDccDevice] = useState(null);

  useEffect(() => {
    function requestPorts() {
      console.log('[DccDeviceDialog] requestPorts', isConnected, open);
      publish(`@ttt/dcc/${layoutId}`, JSON.stringify({ action: 'listPorts', payload: {} }));
    }
    open && isConnected && requestPorts()

  }, [open, isConnected]);

  const handleUpdate = async () => {
    console.log('[DccDeviceDialog] handleUpdate', newDccDevice);
    setDccDevice(newDccDevice);
    onClose()
  }

  return (
    <Dialog onClose={onClose} open={open}>
      <DialogTitle> DCC-EX Serial Connection </DialogTitle>
      <pre>mqtt Status: {isConnected.toString()}</pre>
      <Autocomplete
        sx={{ padding: '1rem', width: '360px' }}
        id="dcc-deviced"
        freeSolo
        options={Array.isArray(ports) ? ports : []} // Added array check
        value={newDccDevice}
        onInputChange={(event, newValue) => {
          setNewDccDevice(newValue);
        }}
        onChange={(event, newValue) => {
          setNewDccDevice(newValue);
        }}
        renderInput={(params) => <TextField {...params} label="DCC Serial Port" />}
      />
      <Button
        size="large"
        startIcon={<SaveIcon />}
        onClick={handleUpdate}>
        Save
      </Button>
    </Dialog>
  )
}

export default DccDeviceDialog;
