# node-raiblocks-pow

Native compiled C Module for Node.js to calculate Proof of Work (PoW) value for a given RaiBlock Hash number (32-byte integer);

Based on [jaimehgb/RaiBlocksWebAssembly](https://github.com/jaimehgb/RaiBlocksWebAssemblyPoW), migrated to use node-gyp.

### Installation

```
npm install --save raiblocks-pow
```

### Usage

```js
var pow = require('raiblocks-pow');

var work = pow('<previous block hash>');
```

### Development

Use `npm test` to check a random block hash against an external threshold validation.
