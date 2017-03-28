import { Component, OnInit } from '@angular/core';
import { DataCollectionService} from '../datacollection.service';

@Component({
  selector: 'app-datacollection',
  templateUrl: './datacollection.component.html',
  styleUrls: ['./datacollection.component.css']
})
export class DataCollectionComponent implements OnInit {
  data : any = [];

  constructor(private dataCollectionService: DataCollectionService) { }

  ngOnInit() {
  	this.data = this.dataCollectionService.postData();
  }

}
