module Jiggle
  class Spring
    attr_accessor :a
    attr_accessor :b
    
    def particles
      [@a, @b]
    end
  end
end
