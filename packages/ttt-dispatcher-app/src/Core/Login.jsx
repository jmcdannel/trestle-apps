import React, { useContext } from 'react';
import { GithubAuthProvider, signInWithPopup, getAuth } from 'firebase/auth'
import { useConnectionStore } from '../Store/useConnectionStore';
import { firebaseApp } from '../firebase';

export const githubAuthProvider = new GithubAuthProvider()

function Login() {

  const auth = getAuth(firebaseApp)
  const isGuest = useConnectionStore(state => state.isGuest);
  const setIsGuest = useConnectionStore(state => state.setIsGuest);

  async function handleGithubLogin() {
    try {
      const resp = await signInWithPopup(auth, githubAuthProvider)
      console.log('Github signin success', resp)
    } catch (err) {
      console.error('Failed signinRedirect', err)
    }
  }

  function handleGuest() {
    setIsGuest(true);
  }

  return (
    <div>
      <h1>Login</h1>
      <pre>isGuest: {isGuest?.toString()}</pre>
      <button onClick={handleGithubLogin}>Login with Github</button>
      <button onClick={handleGuest}>Login as Guest</button>
    </div>
  );
}

export default Login;
