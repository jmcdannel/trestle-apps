import {
  collection,
  setDoc,
  deleteDoc,
  getDoc,
  getDocs,
  query,
  addDoc,
  serverTimestamp,
  onSnapshot,
  where,
  doc,
  deleteField,
} from 'firebase/firestore'
import { db } from '../../firebase';
import { useConnectionStore } from '../../Store/useConnectionStore';
import { useLocoStore } from '../../Store/useLocoStore';


export function useDejaCloud() {
  const initLocos = useLocoStore(state => state.initLocos);
  const layoutId = useConnectionStore(state => state.layoutId);

  async function getLayout() {
    return getDocSnap(doc(db, 'layouts', layoutId))
  }

  async function getDocSnap(docRef) {
    const docSnap = await getDoc(docRef)

    if (docSnap.exists()) {
      console.log('Document data:', docSnap.data(), docSnap.id, docSnap.ref)
      return { ...docSnap.data(), id: docSnap.id }
    } else {
      // docSnap.data() will be undefined in this case
      console.error('No such document!')
    }
  }

  async function init() {
    const locos = []
    const locoCollection = await getDocs(collection(db, `layouts/${layoutId}/locos`))
    locoCollection.forEach((doc) => {
      // doc.data() is never undefined for query doc snapshots
      console.log(doc.id, " => ", doc.data());
      const data = doc.data()
      locos.push({ ...data, id: doc.id, address: data.locoId })
    });

    console.log('locos', locos)
    initLocos(locos, [])

  }

  async function send({ action, payload }) {
    console.log('dejaCloud SEND', action, payload);
    switch (action) {
      case 'throttle':
        sendThrottleUpdate({ action, payload })
        break
      default:
        sendDccCommand({ action, payload })
        break
    }
  }

  async function sendThrottleUpdate({ action, payload }) {
    console.log('dejaCloud SEND', action, payload)
    try {
      const throttle = {
        speed: Math.abs(payload.speed),
        direction: payload.speed > 0 ? true : false,
        timestamp: serverTimestamp(),
      }

      await setDoc(
        doc(
          db,
          `layouts/${layoutId}/throttles`,
          payload.address.toString()
        ),
        throttle,
        { merge: true }
      )

      console.log('throttle written with ID: ', throttle)
    } catch (e) {
      console.error('Error adding throttle: ', e)
    }
  }

  async function sendDccCommand({ action, payload }) {
    // console.log('dejaCloud SEND', action, payload)
    try {
      const command = {
        action,
        payload: JSON.stringify(payload),
        timestamp: serverTimestamp(),
      }

      await addDoc(
        collection(db, `layouts/${layoutId}/dccCommands`),
        command
      )
      // console.log('Document written with ID: ', command)
    } catch (e) {
      console.error('Error adding document: ', e)
    }
  }

  return {
    send,
    getLayout,
    init,
  }
}

export default useDejaCloud;