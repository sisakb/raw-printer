{
  "targets": [
    {
      "target_name": "printer",
      "sources": [ "src/printer.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}