//Captura os dados do Firebase

function GetData() {
  var baseUrl = "https://ssc0721.firebaseio.com/";
  var secret = "spCWs0Wa4jVHSqqtgXGkYzQxjBLPRA7ysrnrjvf4";
  var database = FirebaseApp.getDatabaseByUrl(baseUrl, secret);
  var result = database.getData("/");
  return result;

}


