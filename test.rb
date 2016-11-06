#!/usr/bin/env ruby
require 'rmagick'
require 'Rdmtx'

rdmtx = Rdmtx.new

if ARGV[0]
  image = Magick::Image.read(ARGV[0]).first
  puts "The image contains : "
  puts rdmtx.decode(image, 0)
else
  i = rdmtx.encode("Hello you !!", 5, 5, DmtxSymbolSquareAuto)
  if not i
    puts "Encode failed"
    exit 1
  end
  i.write("output.png")
  puts "Written output.png"
end
