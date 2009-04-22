module Jiggle
  
  class Vector2
    
    def -@
      jgv(0, 0) - self
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
  
  def jgv(x, y)
    Vector2.new(x, y)
  end
  
  def self.jgv(x, y) # WUT
    Vector2.new(x, y)
  end
end
