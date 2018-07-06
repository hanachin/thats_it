require "test_helper"

class ThatsItTest < Test::Unit::TestCase
  def test_cfunc
    assert 2.yield_self { it * it } == 4
  end

  def test_method
    assert m { it * it } == 4
  end

  private

  def m
    yield 2
  end
end
