$(document).ready(function() {
	// This example displays a marker at the center of Australia.
	// When the user clicks the marker, an info window opens.
	// ITB
	// -6.891625, 107.610089

  var locations = [ 
					['ITB', -6.891625, 107.610089],
					['Telkom', -6.974001,107.6281593]
				];
  var startCoordinate = {lat: -6.9034443, lng: 107.5731159};
  var map = new google.maps.Map(document.getElementById('map'), {
    zoom: 12,
    center: startCoordinate
  });
  var temperature = 0 , humidity = 0 , windSpeed = 0 , windDirection = 0, luxIntensity = 0;

  var now = Date();

  var contentString = [
  	  '<div id="content">'+
	      '<div id="siteNotice">'+
	      '</div>'+
	      '<h1 id="firstHeading" class="firstHeading">ZeroDevice - 1</h1>'+
	      '<div> Status : <span id="deviceStats1" class="label label-success">Active</span> </div>' +
	      '<div id="bodyContent">'+
	      '<h4> Realtime Data : </h4>'+
	      '<div class="table-responsive"> ' +
	      '<table class="table">' +
	      '<thead>' +
	      '<tr>' +
	      '<th> Parameter </th> ' +
	      '<th> Value </th> ' +
	      '</tr>' +
	      '</thead>' +
	      '<tbody>' + 
	      '<tr>' +
	      '<th> Temperature </th> ' +
	      '<th> ' + temperature + ' </th> ' +
	      '</tr>' +
	      '<tr>' +
	      '<th> Humidity </th> ' +
	      '<th> ' + humidity + ' </th> ' +
	      '</tr>' +
	      '<tr>' +
	      '<th> Wind Speed </th> ' +
	      '<th> ' + windSpeed + ' </th> ' +
	      '</tr>' +
	      '<tr>' +
	      '<th> Wind Direction </th> ' +
	      '<th> ' + windDirection + ' </th> ' +
	      '</tr>' +
	      '<tr>' +
	      '<th> Lux Intensity </th> ' +
	      '<th> ' + luxIntensity + ' </th> ' +
	      '</tr>' +
	      '</tbody>' +
	      '</table>' +
	      '' +
	      '<p> Data Colletion from this device, <a href="graph.html">'+
	      'Click Here</a> </p>'+
	      	      '<small>' + now + '</small>' +

	      '</div>'+
	      '</div>' 
       
      ,
      
	      '<div id="content">'+
	      '<div id="siteNotice">'+
	      '</div>'+
	      '<h1 id="firstHeading" class="firstHeading">ZeroDevice - 2</h1>'+
	      '<div> Status : <span id="deviceStats1" class="label label-default">Not Active</span> </div>' +
	      '<div id="bodyContent">'+
	      '<h4> Realtime Data : </h4>'+
	      '<div class="table-responsive"> ' +
	      '<table class="table">' +
	      '<thead>' +
	      '<tr>' +
	      '<th> Parameter </th> ' +
	      '<th> Value </th> ' +
	      '</tr>' +
	      '</thead>' +
	      '<tbody>' + 
	      '<tr>' +
	      '<th> Temperature </th> ' +
	      '<th> ' + temperature + ' </th> ' +
	      '</tr>' +
	      '<tr>' +
	      '<th> Humidity </th> ' +
	      '<th> ' + humidity + ' </th> ' +
	      '</tr>' +
	      '<tr>' +
	      '<th> Wind Speed </th> ' +
	      '<th> ' + windSpeed + ' </th> ' +
	      '</tr>' +
	      '<tr>' +
	      '<th> Wind Direction </th> ' +
	      '<th> ' + windDirection + ' </th> ' +
	      '</tr>' +
	      '<tr>' +
	      '<th> Lux Intensity </th> ' +
	      '<th> ' + luxIntensity + ' </th> ' +
	      '</tr>' +
	      '</tbody>' +
	      '</table>' +
	      '' +
	      '<p> Data Colletion from this device, <a href="graph.html">'+
	      'Click Here</a> </p>'+
	      	      '<small>' + now + '</small>' +

	      '</div>'+
	      '</div>' 
      ];


  var infowindow = new google.maps.InfoWindow();
  var marker, i;

  for (var i = 0; i < locations.length ; i++) {
	  marker = new google.maps.Marker({
	    position: new google.maps.LatLng(locations[i][1], locations[i][2]),
	    map: map,
	  });

	  // marker.addListener('click', function() {
	  //   infowindow.setContent(contentString[i]);
	  //   infowindow.open(map, marker);
	  // });
	  google.maps.event.addListener(marker, 'click', (function(marker, i) {
        return function() {
          infowindow.setContent(contentString[i]);
          infowindow.open(map, marker);
        }
      })(marker, i));
  }

  // set number of device
  var numberDevices = locations.length;
  $('#numberDevice').text(numberDevices.toString());


});