module Jiggle

  class Area
    def inspect
      points = "[" + self.particles.map {|p| p.position.to_s }.join(", ") + "]"
      "#<Area: #{points}>"
    end
  end
  
end
