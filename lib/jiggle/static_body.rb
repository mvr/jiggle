module Jiggle
  class StaticBody < Body
    Infinity = 1.0/0 # WTF, Ruby?
    
    def initialize(points, attr={})
      attr[:mass] = Infinity
      super(points, attr)
    end
  end
end
