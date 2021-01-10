from django.db import models
from django.urls import reverse
from django.core.files import File

# Create your models here.

class Algorithm(models.Model):
    name = models.CharField(max_length = 40, unique = True)
    author = models.CharField(max_length = 30)  # nick użytkownika serwisu
    alg_file = models.FileField(upload_to='zip_files/')
    counted = models.BooleanField(default = False)
    num_of_fun = models.IntegerField(default = 0)
    #score = models.FloatField()
    #rank = models.IntegerField()

    def get_absolute_url(self):
        return reverse('algorithms:detail', kwargs={'pk' : self.pk})

    def __str__(self):
        return '{}: {}'.format(self.name, self.author)


class Outcome(models.Model):
    algorithm = models.ForeignKey(Algorithm, on_delete = models.CASCADE)
    dimension = models.IntegerField()
    function = models.IntegerField()
    best = models.CharField(max_length = 30)
    worst = models.CharField(max_length = 30)
    median = models.CharField(max_length = 30)
    mean_float = models.FloatField()
    mean_str = models.CharField(max_length = 30)
    std = models.CharField(max_length = 30)

    def __str__(self):
        return self.algorithm.__str__() + ', fun: ' + str(self.function) + ', dim: ' + str(self.dimension)