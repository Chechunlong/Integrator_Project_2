﻿angular.module("bench", []); 																// Cria o módulo
angular.module("bench").controller("bench_controller", function($scope, $window, $http){	// Cria o montroller
});



new Vue({
    el: '#app',
    data: {
        json: null
    },
    created: function () {
        var _this = this;

		//TODO: set time out
        $.getJSON('API_data_request.php', function (json) {
            _this.json = json;
        });

		
    }
});