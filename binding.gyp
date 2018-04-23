{
  "targets": [
    {
      "target_name": "addon",
      "include_dirs" : [
        "<!(node -e \"require('nan')\")",
        "src"
      ],
      "sources": [
        "src/init.cc",
        "src/mutex.cc"
        ]
    }
  ]
}