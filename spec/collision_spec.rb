require 'spec_helper'

describe Collision do
  before :each do
    @world = World.new
    @world.gravity = jgv(0, 9.8)

    @box = box
    @world.add_body @box
    
    @particle = Particle.new jgv(0, 0)
    @world.add_particle @particle
  end
  
  it "happens when there's a point inside an area" do
    @box.should contain @particle
    
    @world.update(1.0)
    
    @world.collisions.length.should == 1
  end
end
