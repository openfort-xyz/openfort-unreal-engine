# openfort-unreal-engine

## Installation
1. Create `Plugins` folder in the root of your project
2. Copy Openfort plugin there
3. Open your project in Unreal Editor
4. Verify that plugin is installed
        <img width="792" alt="image" src="https://github.com/openfort-xyz/openfort-unreal-engine/assets/58430932/24dd964b-19f4-4b34-9f98-55dad9f52647">

5. Reduild your project

## Components
* `Openfort` - the main component of the plugin, which includes the functionality related to the session keys and has integration with Openfort API

### Functions

#### Session key related
* `GenerateSessionKey` - generates a new key pair for the Openfort session
* `InitializeSessionKey` - initialize Openfort component based on the private key hex (`privateKey` parameter), saved in the game
* `GetSessionPublicKeyHex` - returns public key in the hex format for the current session key pair
* `GetSessionPrivateKeyHex` - returns private key in the hex format for the current session key pair
* `GetSessionAddressHex` - returns address in the hex format for the current session key pair
* `SignMessage` - signs with the session key the message provided as a parameter for the function

#### API Client
* `SetApiKey` - set the api key to the Openfort client to be able to connect to the Openfort API
* `SignatureSession` - sends the signature session request to the Openfort API
* `SignatureTransactionIntent` - sends the signature transaction intent request to the Openfort API


## Getting Started

The following blueprints are provided as an examples of the basic functionality and can be modified according to the projects architecture and needs

### Generate key
![image](https://github.com/openfort-xyz/openfort-unreal-engine/assets/58430932/ae94235a-61dd-46e6-b1ec-7cc1a431c2ad)

### Load key
![image](https://github.com/openfort-xyz/openfort-unreal-engine/assets/58430932/1a21fc4c-4363-4294-8c82-a64839dfbf15)

### Calling Signature Session API method
![image](https://github.com/openfort-xyz/openfort-unreal-engine/assets/58430932/caf0f694-59df-4d08-af8c-0c0e1287f0ce)
![image](https://github.com/openfort-xyz/openfort-unreal-engine/assets/58430932/74af7aab-bde1-48fd-b123-d99fe8ee2971)

