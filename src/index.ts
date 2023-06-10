import bindings from "node-gyp-build"
import path from "path"
import { fileURLToPath } from "url"

const __filename = fileURLToPath(import.meta.url)
const __dirname = path.join(path.dirname(__filename), "..")

const binding = bindings(__dirname)

const PrintRaw = (printer: string, documentName: string, data: string) => {
	binding.printRaw(printer, documentName, data, data.length)
}

export { PrintRaw }
