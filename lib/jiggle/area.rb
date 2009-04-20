module Jiggle

  class Area
    def inspect
      points = "[" + @particles.map {|p| p.position.to_s }.join(", ") + "]"
      "#<Area: #{points}>"
    end

    def contains?(point)
      _contains? point.to_vec
    end
  end
  
end
