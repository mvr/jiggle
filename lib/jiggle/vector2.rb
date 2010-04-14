module Jiggle
  
  class Vector2
    
    def -@
      jgv(0, 0) - self
    end

    def perpendicular
      jgv(-y, x)
    end

    def unit
      jgv(x/length, y/length)
    end
    
    alias :<=> :distance_to
    
    def to_s
      "(#{x}, #{y})"
    end
    
    def inspect
      "#<Vector2 x=#{x} y=#{y}>"
    end
    
    def to_vec
      self
    end

    def to_a
      [x, y]
    end
    alias :to_ary :to_a
  end
end

# It's gross, I know
def jgv(x, y)       
  Jiggle::Vector2.new(x, y)
end
