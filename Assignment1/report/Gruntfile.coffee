module.exports = (grunt) ->
  'use strict'

  grunt.initConfig
    coffee:
      debug:
        files: [
          expand: true
          src: ['js/**/*.coffee']
          ext: '.js'
        ]

    stylus:
      debug:
        files: [
          expand: true
          src: ['css/**/*.styl']
          ext: '.css'
        ]

    jade:
      debug:
        files: [
          expand: true
          src: ['*.jade', '!_*.jade']
          ext: '.html'
        ]

    watch:
      coffee:
        files: ['js/**/*.coffee']
        tasks: ['coffee']

      stylus:
        files: ['css/**/*.styl']
        tasks: ['stylus']

      jade:
        files: ['*.jade']
        tasks: ['jade']

    clean:
      dev: ['*.html', 'css/style.css']

    connect:
      debug:
        options:
          port: 9999
          host: '127.0.0.1'
          base: '.'

  grunt.loadNpmTasks 'grunt-contrib-coffee'
  grunt.loadNpmTasks 'grunt-contrib-stylus'
  grunt.loadNpmTasks 'grunt-contrib-jade'
  grunt.loadNpmTasks 'grunt-contrib-watch'
  grunt.loadNpmTasks 'grunt-contrib-clean'
  grunt.loadNpmTasks 'grunt-contrib-connect'

  grunt.registerTask 'debug', ['coffee:debug', 'stylus:debug', 'jade:debug']
  grunt.registerTask 'dev', ['debug', 'connect:debug', 'watch']
