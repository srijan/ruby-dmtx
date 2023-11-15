require 'rmagick'
require 'Rdmtx'
require 'test/unit'

class TestRdmtx < Test::Unit::TestCase
  def setup
    @rdmtx = Rdmtx.new
  end
  def test_rdmtx
    # remove file
    File.delete("output.png") if File.exist?("output.png")
    # encode
    assert_not_nil(true, @rdmtx.encode("Hello you !!", 5, 5, DmtxSymbolSquareAuto).write("output.png"))
    assert_equal(false, File.empty?("output.png"))
    assert_equal(["Hello you !!"], @rdmtx.decode(Magick::Image.read("output.png").first, 0))
  end
end
