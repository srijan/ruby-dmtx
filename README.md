This is a gem enabled version with some additional features of the ruby
wrapper for libdmtx that can be found at:
https://github.com/dmtx/dmtx-wrappers

The original README file is renamed as libdmtx-README.

Installation
============

    gem install Rdmtx

Dependencies
============

* libdmtx
* rmagick

Example
=======

    require 'rmagick'
    require 'Rdmtx'
    rdmtx = Rdmtx.new
    i = rdmtx.encode("Hello you !!", 5, 5)
    i.write("output.png")

See test.rb for a decode example.

API
===

### Create new Rdmtx object

    rdmtx.new

### Encoding

    rdmtx.encode(String, MarginSize, ModuleSize, SizeRequest)

Creates and returns an rmagick image object by encoding `String` with
(optional arguments) margin `MarginSize`, module `ModuleSize`, and
request a particular size of the output `SizeRequest`.

* `MarginSize` is the margin in number of pixels
* `ModuleSize` is the size of one module in number of pixels
* `SizeRequest` has to be one of pre-defined constants like:
  `DmtxSymbolSquareAuto`, `DmtxSymbol10x10`, etc..
  See [`ext/rdmtx/Rdmtx.c`](ext/rdmtx/Rdmtx.c) for a full listing of the
  possible values.

### Decoding

    rdmtx.decode(Image, Timeout)

Decodes the rmagick image object `Image` within `Timeout`.


Development Environment
=======================

```
sudo apt install software-properties-common
sudo apt-add-repository -y ppa:rael-gc/rvm
sudo apt update
sudo apt install rvm

rvm install 2.7
rvm use 2.7@ruby-dmtx

sudo apt install libmagickwand-dev libdmtx-dev
gem install rmagick

gem build Rdmtx.gemspec
gem install ./
```
