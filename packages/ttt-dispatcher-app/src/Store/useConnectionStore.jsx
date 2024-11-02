import { create } from 'zustand'
import { persist, createJSONStorage } from 'zustand/middleware'
import { immer } from 'zustand/middleware/immer'

export const CONNECTION_STATUS = {
  CONNECTED: 'connected',
  DISCONNECTED: 'disconnected',
  PENDING: 'pending',
  UNKNOWN: 'unknown',
}

const store = immer(

  persist((set, get) => ({
    isGuest: false,
    layoutId: null,
    dccDevice: null, // serial port path to DCC-EX Command
    ports: [],
    actionPorts: [],
    status: CONNECTION_STATUS.DISCONNECTED,
    actionDevices: [],
    dccDeviceStatus: CONNECTION_STATUS.DISCONNECTED,
    actionApiStatus: CONNECTION_STATUS.DISCONNECTED,

    setLayoutId: (layoutId) => {
      set(state => ({
        layoutId,
        dccDevice: null,
        ports: [],
        status: CONNECTION_STATUS.DISCONNECTED,
        dccDeviceStatus: CONNECTION_STATUS.DISCONNECTED,
        actionApiStatus: CONNECTION_STATUS.DISCONNECTED,
        actionDevices: [],
      }))
    },
    setIsGuest: (isGuest) => {
      set({ isGuest })
    },
    setDccDevice: (dccDevice) => {
      set({ dccDevice })
    },
    setPorts: (ports) => set({ ports }),
    setActionPorts: (actionPorts) => set({ actionPorts }),
    addActionDevice: (device) => {
      set(state => ({
        actionDevices: [...state.actionDevices, { ...device, status: CONNECTION_STATUS.DISCONNECTED }]
      }))
    },

    updateActionDevice: (device) => {
      set(state => ({
        actionDevices: [...state.actionDevices.map(d => {
          if (d.id === device.id) {
            return device;
          }
          return d;
        })]
      }))
    },

    updateActionDeviceById: (deviceId) => {
      set(state => ({
        actionDevices: [...state.actionDevices.map(d => {
          if (d.id === deviceId) {
            return d;
          }
          return d;
        })]
      }))
    },

    resetActionDevices: () => {
      set(state => ({
        actionDevices: [...state.actionDevices].map(d => ({
          ...d,
          status: CONNECTION_STATUS.DISCONNECTED
        }))
      }))
    },

    updateActionDeviceStatusByPort: (port, status) => {
      set(state => ({
        actionDevices: [...state.actionDevices.map(d => {
          if (d.port === port) {
            return { ...d, ...{ status } };
          }
          return d;
        })]
      }))
    },

    setStatus: (status) => set({ status }),
    setDccDeviceStatus: (dccDeviceStatus) => set({ dccDeviceStatus }),
    setActionApiStatus: (actionApiStatus) => set({ actionApiStatus }),
    resetConnectionStatus: () => {
      set({
        dccDeviceStatus: CONNECTION_STATUS.DISCONNECTED,
        actionApiStatus: CONNECTION_STATUS.DISCONNECTED,
      })
      get().resetActionDevices()
    }
  }), {
    name: '@ttt/connection-store',
    storage: createJSONStorage(() => localStorage)
  })
)

export const useConnectionStore = create(store)

export default useConnectionStore
