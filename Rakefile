require "bundler/gem_tasks"
require "rake/extensiontask"

task :build => :compile

Rake::ExtensionTask.new("implicit_it") do |ext|
  ext.lib_dir = "lib/implicit_it"
end

task :default => [:clobber, :compile, :spec]
