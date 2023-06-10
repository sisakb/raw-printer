#include <nan.h>
#include "windows.h"

BOOL RawDataToPrinter(LPSTR szPrinterName, LPSTR documentName, LPBYTE lpData, DWORD dwCount)
{
	HANDLE hPrinter;
	DOC_INFO_1 DocInfo;
	DWORD dwJob;
	DWORD dwBytesWritten;

	// Need a handle to the printer.
	if (!OpenPrinter(szPrinterName, &hPrinter, NULL))
		return FALSE;

	// Fill in the structure with info about this "document."
	DocInfo.pDocName = documentName;
	DocInfo.pOutputFile = NULL;
	DocInfo.pDatatype = "RAW";
	// Inform the spooler the document is beginning.
	if ((dwJob = StartDocPrinter(hPrinter, 1, (LPBYTE)&DocInfo)) == 0)
	{
		ClosePrinter(hPrinter);
		return FALSE;
	}
	// Start a page.
	if (!StartPagePrinter(hPrinter))
	{
		EndDocPrinter(hPrinter);
		ClosePrinter(hPrinter);
		return FALSE;
	}
	// Send the data to the printer.
	if (!WritePrinter(hPrinter, lpData, dwCount, &dwBytesWritten))
	{
		EndPagePrinter(hPrinter);
		EndDocPrinter(hPrinter);
		ClosePrinter(hPrinter);
		return FALSE;
	}
	// End the page.
	if (!EndPagePrinter(hPrinter))
	{
		EndDocPrinter(hPrinter);
		ClosePrinter(hPrinter);
		return FALSE;
	}
	// Inform the spooler that the document is ending.
	if (!EndDocPrinter(hPrinter))
	{
		ClosePrinter(hPrinter);
		return FALSE;
	}
	// Tidy up the printer handle.
	ClosePrinter(hPrinter);
	// Check to see if correct number of bytes were written.
	if (dwBytesWritten != dwCount)
		return FALSE;
	return TRUE;
}

void PrintRaw(const Nan::FunctionCallbackInfo<v8::Value> &info)
{

	if (info.Length() < 4)
	{
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}

	if (!info[0]->IsString() || !info[1]->IsString() || !info[2]->IsString() || !info[3]->IsNumber())
	{
		Nan::ThrowTypeError("Wrong arguments");
		return;
	}

	v8::Isolate *isolate = info.GetIsolate();

	v8::String::Utf8Value printer_v8(isolate, info[0]);
	const char *printer = *printer_v8;
	v8::String::Utf8Value docname_v8(isolate, info[1]);
	const char *docname = *docname_v8;
	v8::String::Utf8Value data_v8(isolate, info[2]);
	char *data = *data_v8;
	int length = info[3]->NumberValue(isolate->GetCurrentContext()).FromJust();

	RawDataToPrinter((LPSTR)printer, (LPSTR)docname, (LPBYTE)data, length);
	info.GetReturnValue().Set(true);
}

void Init(v8::Local<v8::Object> exports)
{
	v8::Local<v8::Context> context = exports->CreationContext();
	exports->Set(context,
				 Nan::New("printRaw").ToLocalChecked(),
				 Nan::New<v8::FunctionTemplate>(PrintRaw)
					 ->GetFunction(context)
					 .ToLocalChecked());
}

NODE_MODULE(printer, Init)