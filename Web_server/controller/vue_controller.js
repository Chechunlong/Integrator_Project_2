new Vue({
    el: '#app',
    data: {
        json: null
    },
    created: function () {
		this.requestData();
		window.setInterval(this.requestData, 3000);
    },
	methods: {
		translateColor(color){
			switch(color){
				case "1": return "red"; break;
				case "2": return "blue"; break;
				case "3": return "green"; break;
				default: return "null"; 
			}
		},
		requestData(){
			var _this = this;
			$.getJSON('API_data_request.php', function (json) {
				_this.json = json;
			});
			console.log("Data requested");
		}
	}
});