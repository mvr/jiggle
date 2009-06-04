require 'rake/clean'
require 'rake/loaders/makefile'
require 'spec/rake/spectask'

SOURCES      = FileList.new('src/*.c')
OBJECTS      = SOURCES.ext('.o')
HEADERS      = FileList.new('src/*.h')
LIBRARY      = 'libjiggle.a'
CCOPTIONS    = '-std=c99 -Werror -Wall -pedantic -I. -I.. -Isrc -O3'

CLEAN.include(OBJECTS)
CLEAN.include('libjiggle.a')

task :default => :build

task :build => LIBRARY

file LIBRARY => OBJECTS do
  sh 'ar rcs libjiggle.a src/*.o'
end

rule '.o' => '.c' do |t|
  sh "gcc -c -o #{t.name} #{t.source} #{CCOPTIONS}"
end

Spec::Rake::SpecTask.new do |t|
  t.libs +=  ["lib", "spec"]
end

task :ruby => LIBRARY do
  ext = RUBY_PLATFORM =~ /darwin/ ? "bundle" : "so"
  system "cd ext && ruby extconf.rb && make && mv jiggle_ext.#{ext} ../lib/jiggle_ext.#{ext}"
end
