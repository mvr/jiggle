module Jiggle
  
  class Vector2
    def to_s
      "(#{x}, #{y})"
    end
    def inspect
      "#<Vector2 x=#{x} y=#{y}>"
    end
  end
  
  def jgv(x, y)
    Vector2.new(x, y)
  end
end
