import React, { useContext, useEffect, useState } from 'react';
// import api from '../Api';
import api from '../Shared/api/api';
import { Context } from '../Store/Store';
import log from '../Shared/utils/logger';

function ApiEngine(props) {

  const { onReady } = props;
  const [ state, dispatch ] = useContext(Context);
  const [ init, setInit ] = useState(false);
  const { layout } = state;

  useEffect(() => {
    layout && onReady();
  }, [layout, onReady]);

  useEffect(() => {
    const initialize = async function() {
      try {
        if (init) return;
        console.log('[ApiEngine] connect');
        await api.connect(dispatch);
        setInit(true);
        // const apiSuccess = await api.initialize(dispatch);
        // const wsSuccess = await api.initializeWS();
      } catch (err) {
        setInit(true);
        log.error('api initialization error', err);
      }
    };
    
    !init && initialize();
  }, [onReady, init, dispatch]);


  return (<></>);
}

ApiEngine.defaultProps = {
  onReady: () => { log.info('API Ready'); }
}

export default ApiEngine;