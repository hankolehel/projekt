import { Component, OnInit } from '@angular/core';
import { DataCollectionService} from '../datacollection.service';

@Component({
  selector: 'app-datacollection',
  templateUrl: './datacollection.component.html',
  styleUrls: ['./datacollection.component.css'],
  template: `
  	<input (keyup)="onKey($event)">
  	<p>{{values}}</p>`
})
export class DataCollectionComponent implements OnInit {
  data : any = [];

  constructor(private dataCollectionService: DataCollectionService) { }

  values : number = 0;

  getData(){
    this.values = this.dataCollectionService.getData();
  }

  onKey(event: any) {
    this.dataCollectionService.storeData(parseInt(event.target.value, 10));
  }

  ngOnInit() {
  	this.data = this.dataCollectionService.postData();
  }

}
