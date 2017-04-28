/* tslint:disable:no-unused-variable */

import { TestBed, async, inject } from '@angular/core/testing';
import { DatacollectionService } from './datacollection.service';

describe('DatacollectionService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [DatacollectionService]
    });
  });

  it('should ...', inject([DatacollectionService], (service: DatacollectionService) => {
    expect(service).toBeTruthy();
  }));
});
