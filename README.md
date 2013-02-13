This is a gem enabled version of the ruby wrapper for libdmtx
that can be found at: https://github.com/dmtx/dmtx-wrappers

The original README file is renamed as libdmtx-README.

Installation
============

    gem install Rdmtx

Dependencies
============

* RMagick

Example
=======

    require 'RMagick'
    require 'Rdmtx'
    rdmtx = Rdmtx.new
    i = rdmtx.encode("Hello you !!", 5, 5)
    i.write("output.png")

See test.rb for a decode example.

API
===

    rdmtx.new

Create new Rdmtx object

    rdmtx.encode(String, MarginSize, ModuleSize)

Create and return an RMagick image object by encoding `String`
with margin `MarginSize` and module `ModuleSize`.

    rdmtx.decode(Image, Timeout)

Decode the RMagick image object `Image` within `Timeout`.
