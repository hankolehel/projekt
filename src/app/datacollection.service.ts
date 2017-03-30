import { Injectable } from '@angular/core';
import { Http } from '@angular/http';

@Injectable()
export class DataCollectionService {
	
  constructor(private http: Http) { }
  
  values : number = 0;

  storeData(data : number){
  	this.values = data;
  }

  getData(){
  	return this.values;
  }

  postData(){
  	return [1,2,3,4,5];
  }

}
