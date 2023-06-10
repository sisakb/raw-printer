# raw-printer

A native npm module for printing raw data to a printer on Windows.
Supports sending RAW data including special characters, such as ESC commands to an installed printer driver using Windows' printing queue.

## Installation

The package is available on npm. Prebuilt binaries are provided for Windows using
[node-gyp-build](https://www.npmjs.com/package/node-gyp-build).

```sh
npm install @fork-hu/raw-printer
yarn add @fork-hu/raw-printer
pnpm add @fork-hu/raw-printer
```

## Usage

```typescript
import { PrintRaw } from "@fork-hu/raw-printer"

PrintRaw("EPSON TM-T70II Receipt", "My document", "Hello World!")
```

## API Description

### `PrintRaw` function

```typescript
function PrintRaw(printer: string, documentName: string, data: string): void
```

The `printer` parameter should be the exact name of the printer driver as shown in Windows.
The `data` is the string to be printed, which can contain special characters, such as ESC commands for receipt printers.

## Examples

Usage example for Epson ESC commands:

```typescript
import { PrintRaw } from "@fork-hu/raw-printer"

const ESC = "\x1B"

const alignCenterCommand = ESC + "a" + String.formCharCode(1)

// GS is used for newer commands instead of ESC on EPSON printers
const GS = "\x1D"
const CUT = GS + "V" + "\x01"

// Prints "Hello!", feeds one row and cuts the paper
const data = "Hello!\n" + CUT
PrintRaw("EPSON TM-T70II", "Test document", data)
```
