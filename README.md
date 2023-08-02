![Openfort Protocol][banner-image]

<div align="center">
  <h4>
    <a href="https://www.openfort.xyz/">
      Website
    </a>
    <span> | </span>
    <a href="https://www.openfort.xyz/docs">
      Documentation
    </a>
    <span> | </span>
    <a href="https://www.openfort.xyz/docs/api">
      API Docs
    </a>
    <span> | </span>
    <a href="https://twitter.com/openfortxyz">
      Twitter
    </a>
  </h4>
</div>

[banner-image]: https://strapi-oube.onrender.com/uploads/1_38e40747b6.png

# Openfort UE5 SDK

## Installation
1. Create a `Plugins` folder at the root of your project
2. Copy the Openfort plugin there
3. Open your project in Unreal Editor
4. Verify that the plugin is installed
5. Reduild your project

## Components
* `Openfort` - the main component of the plugin, which includes the functionality related to the session keys and has integration with Openfort API

### Functions

#### Session key related
* `GenerateSessionKey` - generates a new key pair for the Openfort session
* `InitializeSessionKey` - initialize Openfort component based on the private key hex (`privateKey` parameter), saved in the game
* `GetSessionPublicKeyHex` - returns the public key in the hex format for the current session key pair
* `GetSessionPrivateKeyHex` - returns the private key in the hex format for the current session key pair
* `GetSessionAddressHex` - returns the address in the hex format for the current session key pair
* `SignMessage` - signs with the session key the message provided as a parameter for the function

#### API Client
* `SetApiKey` - set the API key to the Openfort client to be able to connect to the Openfort API
* `SignatureSession` - sends the signature session request to the Openfort API
* `SignatureTransactionIntent` - sends the signature transaction intent request to the Openfort API


## Getting Started

The following blueprints are provided as examples of the basic functionality and can be modified according to the architecture and needs of the project

### Generate key
![image](https://github.com/openfort-xyz/openfort-unreal-engine/assets/58430932/0a8377ff-0eff-4fba-b04e-e0ba8c4d64b9)

### Load key
![image](https://github.com/openfort-xyz/openfort-unreal-engine/assets/58430932/6af3b9fb-8243-4dba-8df2-f3899878c279)

### Calling Signature Session API method
![image](https://github.com/openfort-xyz/openfort-unreal-engine/assets/58430932/ef83f86f-c50d-4628-a00e-c8b68cb58744)
![image](https://github.com/openfort-xyz/openfort-unreal-engine/assets/58430932/8848af40-2c0e-4669-9c28-532bbc2bb204)


