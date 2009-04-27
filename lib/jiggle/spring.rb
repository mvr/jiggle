module Jiggle
  class Spring
    attr_reader :a
    attr_reader :b
    
    def particles
      [@a, @b]
    end
  end
end
