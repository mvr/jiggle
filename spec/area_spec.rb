require 'spec_helper'

describe Area do
  before :each do
    @area = box.areas[0] # Meh.
  end
  
  it "#contains? a vector2" do
    @vector = jgv(0, 0)
    @area.should contain @vector
  end
  
  it "#contains? a particle" do
    @particle = Particle.new jgv(0, 0)
    @area.should contain @particle    
  end
end
